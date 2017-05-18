//tests for the client

public class TestClient {
	
	public static void main(String[] args) throws InterruptedException
	{
		Client c = new Client("linux2",10050,"p");
		System.out.println("Status:"+c.open());
		System.out.println("Status:"+c.startAIAI("linux2",10051,"p",2,2));
		while(true)
		{
			//Thread.sleep(2000);
			int m = c.getMove();
			if(m == 1)
			{
				System.out.println("WHITE WON");	
				break;
			}

			if(m == 2)
			{
				System.out.println("BLACK Won");
				break;
			}
		}
				
		System.out.println("Status:"+c.close());

	}
}
