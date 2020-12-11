import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.text.SimpleDateFormat;
import java.util.Date;

public class DaytimeImpl extends UnicastRemoteObject implements Daytime {
    public DaytimeImpl() throws RemoteException {
}

    public String getDaytime() {
		return "Servertime: " + new SimpleDateFormat("dd.MM.yyyy HH:mm:ss:SSS").format(new Date());
    }
}