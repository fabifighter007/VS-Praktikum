
public class FigurenThread {
	public FigurenThread() {
		Figur f1 = new FigurenHandler();
		Thread leser1 = new Leser(f1);
		leser1.start();
		
		Thread schreiber1 = new Schreiber(f1);
		schreiber1.start();
		
		Figur f2 = new FigurenHandler();
		Thread leser2 = new Leser(f2);
		leser2.start();
		
		Thread schreiber2 = new Schreiber(f2);
		schreiber2.start();
	}
	
	public static void main(String args[]) {
		new FigurenThread();
	}
}
