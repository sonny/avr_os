/*
 * uart.c
 *
 * Created: 7/8/2013 8:12:47 PM
 *  Author: Greg Cook
 *
 * Uart (originally named Serial) implementation. Implements RTS/CTS 
 * HW flow control.
 */

#include "system.h"
#include "uart.h"
#include "atmega/bits_atmega.h"
#include <util/atomic.h>

/**
   This website explains serial HW flow control properly
   http://www.tldp.org/HOWTO/Serial-HOWTO-19.html
   Surprisingly, it is somewhat difficult to find usable documentation
   on the subject.
*/
static ring_buffer_t TX_buffer0;
static ring_buffer_t RX_buffer0;

static int serial0_putc(char c, FILE*);
static int serial0_getc(FILE*);

// Set serial0 and serial1 to the beginning
// of USART0 and USART1 devices in memory
static uart_t __serial0 = {
  .regs = (usart_atmega_regs_t *)(0xC0),
  .tx_buffer = &TX_buffer0,
  .rx_buffer = &RX_buffer0,
  .rts = GPIO_PIN(B,7),
  .cts = GPIO_PIN(B,6),
  .putc = serial0_putc,
  .getc = serial0_getc
};

uart_t * const UART0 = &__serial0;

#if USE_UART1
static uart_t __serial1 = {
  .regs = (usart_atmega_regs_t *)(0xC8)
};

uart_t * const UART1 = &__serial1;
#endif

/**
 * Calculate baud register values based on desired baud rate
 * @param baud Baud rate to set
 * @return 16-bit register value for ubbrX register
 * @note Normal caveats apply when using 16-bit reg values
 */
static inline uint16_t calc_baud_ubbrx(uint32_t baud) {
  return ((SYSCLOCK / ((uint32_t)16*(uint32_t)baud)) -1);
}

/**
 * Initialize UART device
 * @param uart UART device
 * @param baud Baud rate to set
 * @return void
 */
void uart_init(uart_t *uart, uint32_t baud) {
  uint16_t ubrrx = calc_baud_ubbrx(baud);
  usart_atmega_init(uart->regs, ubrrx);
  Ringbuffer.init(uart->tx_buffer);
  Ringbuffer.init(uart->rx_buffer);

#if UART_HW_FLOW_CTL
  gpio_pin_set_direction(uart->cts, in);
  gpio_pin_set_direction(uart->rts, out);
  gpio_pin_set_value(uart->rts, unset);	
#endif

  // initialize put char function if one is configured
  if (uart->putc) { fdevopen(uart->putc, uart->getc); }
}

/**
 * Returns true if it is OK to send data to the other side
 * @param uart UART device
 * @return true if OK to send
 */
static inline bool uart_read_ready_to_receive(const uart_t *uart) {
  return !gpio_pin_read_pin(uart->cts);
}

/**
 * Set ready to receive on our end
 * @param uart UART device
 * @return void
 */
static inline void uart_assert_ready_to_receive(const uart_t *uart) {
  gpio_pin_set_value(uart->rts, unset);
}

/**
 * Set NOT ready to recieve on our end
 * @param uart UART device
 * @return void
 */
static inline void uart_deassert_ready_to_receive(const uart_t *uart) {
  gpio_pin_set_value(uart->rts, set);
}

/**
 * putc function for UART0
 * @param c Char to put onto device
 * @param f FILE pointer for device
 * @return 0 if okay, error of some sort if not
 */
static int serial0_putc(char c, FILE *f) {
  (void)f;

  // block until other side asserts RTS
  NONATOMIC_BLOCK(NONATOMIC_RESTORESTATE) {
    while(!uart_read_ready_to_receive(UART0)) ;
  }
	
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    Ringbuffer.insert(UART0->tx_buffer, c, true);

    // enable data register empty irq
    REG_SETBIT(UART0->regs->UCSRxB, UDRE0); 
  }
  return 0;
}

/**
 * Decides when it is time to set and unset Ready-to-Receive for UART0
 * @param void
 * @return void
 */
static void uart_handle_rx_full(void) {
  if (Ringbuffer.almost_full(UART0->rx_buffer))
    uart_deassert_ready_to_receive(UART0);
  else
    uart_assert_ready_to_receive(UART0);
}

/**
 * getc function for UART0
 * @param f FILE pointer for UART0
 * @return next char in stream or EOF if end of stream
 */
static int serial0_getc(FILE *f) {
  (void)f;
  int result = _FDEV_EOF;

  if (!Ringbuffer.empty(UART0->rx_buffer))
    result = Ringbuffer.remove(UART0->rx_buffer);
  uart_handle_rx_full();

  return result;
}

// usart data register empty interrupt
ISR(USART0_UDRE_vect) 
{
  if (!Ringbuffer.empty(UART0->tx_buffer))
    UART0->regs->UDRx = Ringbuffer.remove(UART0->tx_buffer);
  else
    REG_CLRBIT(UART0->regs->UCSRxB, UDRE0);
}

// usart receive complete
ISR(USART0_RX_vect)
{
  // pray that this stops the other end in time
  uart_handle_rx_full();
  Ringbuffer.insert(UART0->rx_buffer, UART0->regs->UDRx, false);
}
