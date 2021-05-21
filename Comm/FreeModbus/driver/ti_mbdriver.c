#include <string.h>
#include <termios.h>
#include "mbdriver.h"
#include <linux/serial.h>
#include <sys/ioctl.h>

#define TIOCGRS485      0x542E
#define TIOCSRS485      0x542F

/**********************************************************************
 * @brief   UART初始化
 * @param   *Uart   UART
 * @return	none
 *********************************************************************/
uint8_t xMB_UartInit(const sUART_Def *Uart, int fd)
{
    struct termios tios = {0};
    speed_t speed;
    int32_t err;

    struct serial_rs485 rs485conf;

    rs485conf.flags |= SER_RS485_ENABLED;
    rs485conf.flags |= SER_RS485_RTS_ON_SEND;
    //rs485conf.flags &= ~(SER_RS485_RTS_ON_SEND);

    rs485conf.flags &= ~(SER_RS485_RTS_AFTER_SEND);
    //rs485conf.flags rs485conf.flags |= SER_RS485_RX_DURING_TX;

    rs485conf.delay_rts_after_send = 0;

    if( (err = ioctl(fd, TIOCSRS485, &rs485conf) )< 0 )
    {
       printf("ioctl error %d\n", err);
    }

        //printf("ioctl error\n");
//        close(Uart->fd);
//        return 0;


    switch (Uart->baud)
    {
	case 110:
		speed = B110;
		break;
	case 300:
		speed = B300;
		break;
	case 600:
		speed = B600;
		break;
	case 1200:
		speed = B1200;
		break;
	case 2400:
		speed = B2400;
		break;
	case 4800:
		speed = B4800;
		break;
	case 9600:
		speed = B9600;
		break;
	case 19200:
		speed = B19200;
		break;
	case 38400:
		speed = B38400;
		break;
	case 57600:
		speed = B57600;
		break;
	case 115200:
		speed = B115200;
		break;
	default:
		speed = B9600;
	}
	/* Set the baud rate */
	if((cfsetispeed(&tios, speed) < 0) || (cfsetospeed(&tios, speed) < 0))
    {
        printf("cfsetispeed error\n");
		return 0;
	}
    /* C_CFLAG      Control options
	   CLOCAL       Local line - do not change "owner" of port
	   CREAD        Enable receiver
	*/
	tios.c_cflag |= (CREAD | CLOCAL);
	/* CSIZE, HUPCL, CRTSCTS (hardware flow control) */

	/* Set data bits (5, 6, 7, 8 bits)
	   CSIZE        Bit mask for data bits
	*/
	tios.c_cflag &= ~CSIZE;
	switch (Uart->data_bit) {
	case 5:
		tios.c_cflag |= CS5;
		break;
	case 6:
		tios.c_cflag |= CS6;
		break;
	case 7:
		tios.c_cflag |= CS7;
		break;
	case 8:
	default:
		tios.c_cflag |= CS8;
		break;
	}

	/* Stop bit (1 or 2) */
	if (Uart->stop_bit == 1)
		tios.c_cflag &=~ CSTOPB;
	else /* 2 */
		tios.c_cflag |= CSTOPB;

	/* PARENB       Enable parity bit
	   PARODD       Use odd parity instead of even */
    if(Uart->parity == 0)   //None
    {
		tios.c_cflag &=~ PARENB;
    }
    else if (Uart->parity == 2)  //Even
    {
		tios.c_cflag |= PARENB;
		tios.c_cflag &=~ PARODD;
    }
    else  /* odd */
    {
		tios.c_cflag |= PARENB;
		tios.c_cflag |= PARODD;
	}

	/* Read the man page of termios if you need more information. */

	/* This field isn't used on POSIX systems
	   tios.c_line = 0;
	*/

	/* C_LFLAG      Line options

	   ISIG Enable SIGINTR, SIGSUSP, SIGDSUSP, and SIGQUIT signals
	   ICANON       Enable canonical input (else raw)
	   XCASE        Map uppercase \lowercase (obsolete)
	   ECHO Enable echoing of input characters
	   ECHOE        Echo erase character as BS-SP-BS
	   ECHOK        Echo NL after kill character
	   ECHONL       Echo NL
	   NOFLSH       Disable flushing of input buffers after
	   interrupt or quit characters
	   IEXTEN       Enable extended functions
	   ECHOCTL      Echo control characters as ^char and delete as ~?
	   ECHOPRT      Echo erased character as character erased
	   ECHOKE       BS-SP-BS entire line on line kill
	   FLUSHO       Output being flushed
	   PENDIN       Retype pending input at next read or input char
	   TOSTOP       Send SIGTTOU for background output

	   Canonical input is line-oriented. Input characters are put
	   into a buffer which can be edited interactively by the user
	   until a CR (carriage return) or LF (line feed) character is
	   received.

	   Raw input is unprocessed. Input characters are passed
	   through exactly as they are received, when they are
	   received. Generally you'll deselect the ICANON, ECHO,
	   ECHOE, and ISIG options when using raw input
	*/

	/* Raw input */
	tios.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

	/* C_IFLAG      Input options

	   Constant     Description
	   INPCK        Enable parity check
	   IGNPAR       Ignore parity errors
	   PARMRK       Mark parity errors
	   ISTRIP       Strip parity bits
	   IXON Enable software flow control (outgoing)
	   IXOFF        Enable software flow control (incoming)
	   IXANY        Allow any character to start flow again
	   IGNBRK       Ignore break condition
	   BRKINT       Send a SIGINT when a break condition is detected
	   INLCR        Map NL to CR
	   IGNCR        Ignore CR
	   ICRNL        Map CR to NL
	   IUCLC        Map uppercase to lowercase
	   IMAXBEL      Echo BEL on input line too long
	*/
    if(Uart->parity == 0) //none
    {
		tios.c_iflag &= ~INPCK;
    }
    else
    {
		tios.c_iflag |= INPCK;
	}

	/* Software flow control is disabled */
	tios.c_iflag &= ~(IXON | IXOFF | IXANY);

	/* C_OFLAG      Output options
	   OPOST        Postprocess output (not set = raw output)
	   ONLCR        Map NL to CR-NL

	   ONCLR ant others needs OPOST to be enabled
	*/

	/* Raw ouput */
	tios.c_oflag &=~ OPOST;

	/* C_CC         Control characters
	   VMIN         Minimum number of characters to read
	   VTIME        Time to wait for data (tenths of seconds)

	   UNIX serial interface drivers provide the ability to
	   specify character and packet timeouts. Two elements of the
	   c_cc array are used for timeouts: VMIN and VTIME. Timeouts
	   are ignored in canonical input mode or when the NDELAY
	   option is set on the file via open or fcntl.

	   VMIN specifies the minimum number of characters to read. If
	   it is set to 0, then the VTIME value specifies the time to
	   wait for every character read. Note that this does not mean
	   that a read call for N bytes will wait for N characters to
	   come in. Rather, the timeout will apply to the first
	   character and the read call will return the number of
	   characters immediately available (up to the number you
	   request).

	   If VMIN is non-zero, VTIME specifies the time to wait for
	   the first character read. If a character is read within the
	   time given, any read will block (wait) until all VMIN
	   characters are read. That is, once the first character is
	   read, the serial interface driver expects to receive an
	   entire packet of characters (VMIN bytes total). If no
	   character is read within the time allowed, then the call to
	   read returns 0. This method allows you to tell the serial
	   driver you need exactly N bytes and any read call will
	   return 0 or N bytes. However, the timeout only applies to
	   the first character read, so if for some reason the driver
	   misses one character inside the N byte packet then the read
	   call could block forever waiting for additional input
	   characters.

	   VTIME specifies the amount of time to wait for incoming
	   characters in tenths of seconds. If VTIME is set to 0 (the
	   default), reads will block (wait) indefinitely unless the
	   NDELAY option is set on the port with open or fcntl.
	*/
	/* Unused because we use open with the NDELAY option */
	tios.c_cc[VMIN] = 0;
	tios.c_cc[VTIME] = 0;

    tcflush(fd, TCIOFLUSH);

    if( (err = tcsetattr(fd, TCSANOW, &tios) )< 0)
    {
        printf("tcsetattr error %d \n", err);
        return 0;
    }
    return 1;
}

/**********************************************************************
 * @brief   485总线接收或者发送
 * @param   *Uart   UART
 * @param   mode    0：接收
 * 					1：发送
 * @return	none
 *********************************************************************/
void MB_SendOrRecive(const sUART_Def *Uart, eUART_EN mode, int fd)
{
    struct serial_rs485 rs485conf;
    int32_t err;

    rs485conf.flags |= SER_RS485_ENABLED;
    if(mode == UART_TX_EN)
    {
        rs485conf.flags |= SER_RS485_RTS_ON_SEND;
    }
    else if(mode == UART_RX_EN)
    {
        rs485conf.flags &= ~(SER_RS485_RTS_ON_SEND);
    }
    if( (err = ioctl(fd, TIOCSRS485, &rs485conf) )< 0 )
    {
       printf("ioctl error %d\n", err);
    }
}
