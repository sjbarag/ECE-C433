import java.io.*;
import java.net.*;
import java.util.*;

public final class WebServer
{
	public static void main(String argv[]) throws Exception
	{
		/* port number to be used for the server */
		final int PORT = 55000;

		/* establish the listen socket */
		ServerSocket sock = new ServerSocket(PORT);
		Socket toSend = null;

		while (true)
		{
			/* listen for a TCP connection request */
			// TODO: try to remove the try/catch
			try {
				toSend = sock.accept();
			} catch ( Exception e ) {
				System.out.printf("E/WebServer: Error accepting TCP request!\n");
			}

			/* construct an object to process the HTTP request message. */
			HttpRequest request = new HttpRequest(toSend);

			/* Create a new thread to process the request */
			Thread thread = new Thread(request);

			/* start the tread */
			thread.start();
		}
	}
}

final class HttpRequest implements Runnable
{
	final static String CRLF = "\r\n";
	Socket sock;

	/**
	 * Public constructor.
	 * @param  socket   the socket that accepted the request
	 * @return          object of type HttpRequest
	 */
	public HttpRequest(Socket socket) throws Exception
	{
		this.sock = socket;
	}

	/**
	 * Calls processRequest() or prints an exception.
	 */
	 public void run()
	 {
	 	try {
			processRequest();
		} catch (Exception e) {
			System.out.println("E/HttpRequest.run(): " +e);
		}
	}

	/**
	 * Processes an HTTP request.
	 */
	private void processRequest() throws Exception
	{
		/* get a reference to the socket's input and output streams */
		InputStream is = sock.getInputStream();
		DataOutputStream os = new DataOutputStream(sock.getOutputStream());

		/* set up input stream filters */
		InputStreamReader in = new InputStreamReader(is);
		BufferedReader br = new BufferedReader(in);

		/* get the request line of the HTTP reqeuest message */
		String requestLine = br.readLine();

		/* display the request line */
		System.out.println();
		System.out.println(requestLine);

		/* get and display the header lines */
		String headerLine = null;
		while( (headerLine = br.readLine()).length() != 0 )
			System.out.println(headerLine);

		/* close streams and socket */
		os.close();
		br.close();
		sock.close();
	}
}
