import java.rmi.Naming;
import java.text.SimpleDateFormat;
import java.util.Date;

public class DaytimeClient {
	public static void main(String args[]) throws Exception {
		printTime("Starttime Client: ");
	    String host = args[0];
	    String text = args[1];
	    Daytime remote = (Daytime) Naming.lookup("//" + host + "/echo");
	    
	    String received = remote.getDaytime();
	    System.out.println(received);
	    printTime("Endtime Client: ");
	}
	
	private static void printTime(String s) {
		System.out.println(s + new SimpleDateFormat("dd.MM.yyyy HH:mm:ss:SSSS").format(new Date()));
	}
}