import gnu.io.*;
import java.util.HashMap;
import java.util.Enumeration;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.IOException;
import java.util.TooManyListenersException;
import java.util.Scanner;
import java.util.Arrays;
import javax.xml.bind.DatatypeConverter;


public class SerialPacketConnTest implements SerialPortEventListener
{
	
	final static String SERIAL_PORT_NAME = "COM3";
	private HashMap portList;
	private SerialPort serialPort;
	public InputStream input;
	public OutputStream output;

	//Protocol Constants
	final static byte CMD_OFF = 0x01;
	final static byte CMD_OFF_RSP = 0x02;
	final static byte CMD_SOLID = 0x03;
	final static byte CMD_SOLID_RSP = 0x04;
	final static byte CMD_CYCLE = 0x05;
	final static byte CMD_CYCLE_RSP = 0x06;
	final static byte CMD_SOLIDCYCLE = 0x07;
	final static byte CMD_SOLIDCYCLE_RSP = 0x08;
	final static byte CMD_GETSTATUS = 0x09;
	final static byte CMD_GETSTATUS_RSP = 0x0A;

	
	public SerialPacketConnTest()
	{
		portList = new HashMap();
	}
	
	public static void main(String[] args) throws IOException
	{
		System.out.println("Starting Serial Port");
		System.out.println("Com Port name: " + SERIAL_PORT_NAME);
		SerialPacketConnTest command = new SerialPacketConnTest();
		command.connect();
		command.initIOStreams();
		command.initListener();
		
		Scanner scan = new Scanner(System.in);
		scan.nextLine();
		byte[] message = { 0x03, 0x11, 0x22, 0x03, 0x33, 0x56, 0x00 };
		System.out.println("Sending: " + DatatypeConverter.printHexBinary(message));
		command.output.write(message, 0, 7);
		command.output.flush();
		scan.nextLine();
		command.disconnect();
		
	}
	
	public void connect()
	{
		System.out.println("Connecting...");
		CommPortIdentifier selectedPort = null;
		try
		{
			selectedPort = CommPortIdentifier.getPortIdentifier(SERIAL_PORT_NAME);
			CommPort commPort = null;
		}
		catch(NoSuchPortException e)
		{
			System.out.println("Port " + SERIAL_PORT_NAME + " doesn't exist");
			return;
		}
		
		try
		{
			System.out.println("Opening Com Port...");
			serialPort = (SerialPort)selectedPort.open("RGBCommand", 2000);
			serialPort.setSerialPortParams(9600, SerialPort.DATABITS_8, SerialPort.STOPBITS_1,SerialPort.PARITY_NONE);
			System.out.println("ComPort Opened.");
		}
		catch(PortInUseException e)
		{
			System.out.println("Com Port In Use");
		}
		catch(Exception e)
		{
			System.out.println("Failed to open port");
		}
	}
	
	public void disconnect()
	{
		try
		{
			serialPort.removeEventListener();
			serialPort.close();
			input.close();
			output.close();
			System.out.println("Disconnected");
		}
		catch(Exception e)
		{
			System.out.println("Error disconnecting");
		}
	}
	
	public boolean initIOStreams()
	{
		System.out.println("Initializing IO Streams");
		boolean successful = false;
		try
		{
			input = serialPort.getInputStream();
			output = serialPort.getOutputStream();
			successful = true;
			return successful;
		}
		catch(IOException e)
		{
			System.out.println("Error getting streams");
			return successful;
		}
	}
	
	public void initListener()
	{
		System.out.println("Initializing Listener");
		try
		{
			serialPort.addEventListener(this);
			serialPort.notifyOnDataAvailable(true);
		}
		catch(TooManyListenersException e)
		{
			System.out.println("Too Many Listeners");
		}
	}
	
	public void portSearch()
	{
		Enumeration ports = CommPortIdentifier.getPortIdentifiers();
		while(ports.hasMoreElements())
		{
			CommPortIdentifier curPort = (CommPortIdentifier)ports.nextElement();
			if(curPort.getPortType() == CommPortIdentifier.PORT_SERIAL)
			{
				portList.put(curPort.getName(), curPort);
			}
		}
	}
	
	public void serialEvent(SerialPortEvent event)
	{
		System.out.println("Serial Event");
		if(event.getEventType() == SerialPortEvent.DATA_AVAILABLE)
		{
			try
			{
				int numBytes = input.available();
				if(numBytes > 0)
				{
					byte[] buffer = new byte[numBytes];
					int interval = 0;
					input.read(buffer, 0, numBytes);
					System.out.println("Response: " + DatatypeConverter.printHexBinary(buffer));
				}
			}
			catch(Exception e)
			{
				System.out.println("Failed to read data");
			}
		}
		return;
	}
}
