package aufgabe2;

import java.io.IOException;

public class Test extends Thread {
	
	static UniqueId t = new UniqueId("id.dat");

	
	public static void main(String[] args) {
		try {
			t.init(10000);
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		Thread t1 = new Test();
		t1.start();
		
		
		Thread t2 = new Test();
		t2.start();
		
		
		Thread t3 = new Test();
		t3.start();
		
		Thread t4 = new Test();
		t4.start();
		
		Thread t5 = new Test();
		t5.start();
		
	}


	public void run() {
		String name = getName();
		for(int i=0; i<10;i++) {
			try {
				String res = name + ": " + t.getNext();
				System.out.println(res);
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
}
