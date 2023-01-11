/*
 * Pi433 socket demo (for sockets with PT2262 or compatible)
 *
 * Copyright (c) 2017 Marcus Wolf <marcus.wolf@wolf-entwicklungen.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 */

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#define REPETITIONS 4
#define PAYLOAD_LEN 32*REPETITIONS

static void pabort(const char *s)
{
	perror(s);
	abort();
}

static	uint8_t bits = 8;
static	uint32_t speed = 500000;
static	uint16_t delay = 0;
static	uint8_t tx[PAYLOAD_LEN + 4];

static int setupRFParameters(int fd)
{
	int i, ret;

	uint8_t tx[] = {
	/* attention - do not modify this parameters unless you know what you do */
	/* wrong settings may lead to violation of law */
	0x81,0x04,	// set standby mode (just for safety)
	0x82,0x08,	// OOK, packet, no shaping
	0x91,0x9f,	// set up power amps
	0x98,0x08,	// antenna impedance (default: 50Ohm)
	0x87,0x6c,	// frequency
	0x88,0x7a,	// frequency
	0x89,0xe1,	// frequency
	0x83,0x25,	// bit rate
	0x84,0xe0,	// bit rate
	0x92,0x09,	// pa ramp (default: 40Âµs)
	};

	for (i = 0; i < ARRAY_SIZE(tx); i+=2)
	{
		struct spi_ioc_transfer tr =
		{
			.tx_buf = (int)tx+i,
			.rx_buf = 0,
			.len = 2,
			.delay_usecs = delay,
			.speed_hz = speed,
			.bits_per_word = bits,
		};

		ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
		if (ret < 1)
			pabort("can't send spi message to setup RF parameters");
	}
	printf("RF parameter set up\n");
}

static int setupMessageParameters(int fd)
{
	int i, ret;

	uint8_t tx[] = {
	0xBC,0x80,		// startup condition (default: fifo not empty)
	0xB8,PAYLOAD_LEN,	// payload length
	0xAC,0x00,		// preamble size
	0xAD,0x00,		// preamble size
	0xB7,0x10,		// fixed length, CRC on, no adress filtering
	0xAE,0x80,		// sync pattern config (default: on, 3 bytes)
	};

	for (i = 0; i < ARRAY_SIZE(tx); i+=2)
	{
		struct spi_ioc_transfer tr =
		{
			.tx_buf = (int)tx+i,
			.rx_buf = 0,
			.len = 2,
			.delay_usecs = delay,
			.speed_hz = speed,
			.bits_per_word = bits,
		};

		ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
		if (ret < 1)
			pabort("can't send spi message to setup message parameters");
	}
	printf("message parameter set up\n");
}


static int prepareTxDataOn(void)
{
	int i;

	/* stop transmission (in case stil active) */
	tx[0] = 0x81; tx[1] = 0x04;

	/* load fifo */
	for (i=0; i<REPETITIONS; i++)
	{
		/* This dataset is intended to control sockets
		   equipped with a PT2262 chip or comparable.
		   The second column contains the data.
		   0x88 = on, 0x8E = high-z.
		   Sockets of different companies use different
		   wiring on the PT chip. So maybe you need to tune.
		   Left comments shows most commen combination */
                tx[32*i+ 2] = 0x80; tx[32*i+ 3] = 0x88;     // 1            // 8
                tx[32*i+ 4] = 0x80; tx[32*i+ 5] = 0x88;     // 2            // 7
                tx[32*i+ 6] = 0x80; tx[32*i+ 7] = 0x88;     // 3            // 6
                tx[32*i+ 8] = 0x80; tx[32*i+ 9] = 0x88;     // 4            // 5
                tx[32*i+10] = 0x80; tx[32*i+11] = 0x88;     // 5            // open
                tx[32*i+12] = 0x80; tx[32*i+13] = 0x8E;     // 6            // open
                tx[32*i+14] = 0x80; tx[32*i+15] = 0x8E;     // A            // GND
                tx[32*i+16] = 0x80; tx[32*i+17] = 0x88;     // B            // GND
                tx[32*i+18] = 0x80; tx[32*i+19] = 0x8E;     // C            // Teste 1
                tx[32*i+20] = 0x80; tx[32*i+21] = 0x8E;     // D            // Teste 2
                tx[32*i+22] = 0x80; tx[32*i+23] = 0x88;     // L            // Taste 4?
                tx[32*i+24] = 0x80; tx[32*i+25] = 0x8E;     // R            // Taste 2
                tx[32*i+26] = 0x80; tx[32*i+27] = 0x80;     // S1           // S1
                tx[32*i+28] = 0x80; tx[32*i+29] = 0x00;     // S2           // S2
                tx[32*i+30] = 0x80; tx[32*i+31] = 0x00;     // S3           // S3
                tx[32*i+32] = 0x80; tx[32*i+33] = 0x00;     // S4           // S4
	}

	/* start transmission */
	tx[32*i+0] = 0x81; tx[32*i+1] = 0x0c;
}

static int prepareTxDataOff(void)
{
	int i;

	/* stop transmission (in case stil active) */
	tx[0] = 0x81; tx[1] = 0x04;

	/* load fifo */
	for (i=0; i<REPETITIONS; i++)
	{
		/* This dataset is intended to control sockets
		   equipped with a PT2262 chip or comparable.
		   The second column contains the data.
		   0x88 = on, 0x8E = high-z.
		   Sockets of different companies use different
		   wiring on the PT chip. So maybe you need to tune.
		   Left comments shows most commen combination */
                tx[32*i+ 2] = 0x80; tx[32*i+ 3] = 0x88;     // 1            // 8
                tx[32*i+ 4] = 0x80; tx[32*i+ 5] = 0x88;     // 2            // 7
                tx[32*i+ 6] = 0x80; tx[32*i+ 7] = 0x88;     // 3            // 6
                tx[32*i+ 8] = 0x80; tx[32*i+ 9] = 0x88;     // 4            // 5
                tx[32*i+10] = 0x80; tx[32*i+11] = 0x88;     // 5            // open
                tx[32*i+12] = 0x80; tx[32*i+13] = 0x8E;     // 6            // open
                tx[32*i+14] = 0x80; tx[32*i+15] = 0x8E;     // A            // GND
                tx[32*i+16] = 0x80; tx[32*i+17] = 0x88;     // B            // GND
                tx[32*i+18] = 0x80; tx[32*i+19] = 0x8E;     // C            // button 1
                tx[32*i+20] = 0x80; tx[32*i+21] = 0x8E;     // D            // button 3
                tx[32*i+22] = 0x80; tx[32*i+23] = 0x8E;     // left button  // button 4
                tx[32*i+24] = 0x80; tx[32*i+25] = 0x88;     // right button // button 2
                tx[32*i+26] = 0x80; tx[32*i+27] = 0x80;     // sync 1       // sync 1
                tx[32*i+28] = 0x80; tx[32*i+29] = 0x00;     // sync 2       // sync 2
                tx[32*i+30] = 0x80; tx[32*i+31] = 0x00;     // sync 3       // sync 3
                tx[32*i+32] = 0x80; tx[32*i+33] = 0x00;     // sync 4       // sync 4
	}

	/* start transmission */
	tx[32*i+0] = 0x81; tx[32*i+1] = 0x0c;
}


static int transferData(int fd, unsigned int packetNr)
{
	int i,ret;

	for (i = 0; i < ARRAY_SIZE(tx); i+=2)
	{
		struct spi_ioc_transfer tr =
		{
			.tx_buf = (int)tx+i,
			.rx_buf = 0,
			.len = 2,
			.delay_usecs = delay,
			.speed_hz = speed,
			.bits_per_word = bits,
		};

		ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
		if (ret < 1)
			pabort("can't send spi message to trasmit data");
	}
}

int main(int argc, char *argv[])
{
	int i;
	int fd;
        int packetNr = 0;


	printf("*** Pi433 socket demo by Marcus Wolf ***\n");

	/* open and configure SPI interface */
	fd = open("/dev/spidev0.0", O_RDWR);
	if (fd < 0)
		pabort("can't open spi. Maybe you need to enable spi by calling \"raspi-config\"");

	setupRFParameters(fd);
	setupMessageParameters(fd);

	printf("\nPress enter to toggle on/off state of the socket.\n");
	printf("^C to quit\n");

	while(1)
	{
		sleep(1); // wait a sec for transmission to complete
		getchar();
		printf("On\n");
		prepareTxDataOn();
		transferData(fd,packetNr++);
		sleep(1); // wait a sec for transmission to complete
		getchar();
		printf("Off\n");
		prepareTxDataOff();
		transferData(fd,packetNr++);
	}

	close(fd);

	return 0;
}
