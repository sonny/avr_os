/*
 * spi_atmega.h
 *
 * Created: 11/2/2013 11:13:06 PM
 *  Author: Greg Cook
 */ 


#ifndef SPI_ATMEGA_H_
#define SPI_ATMEGA_H_

typedef enum { SPI_SLAVE, SPI_MASTER } spi_mode_t;
typedef enum { SPI_SS_SELECT, SPI_SS_DESELCT } spi_ss_t;

void spi_atmega_init(spi_mode_t);
void spi_atmega_set_data(uint8_t);
void spi_atmega_ss(spi_ss_t);

#endif /* SPI_ATMEGA_H_ */