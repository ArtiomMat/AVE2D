import java.util.Scanner;

class A
{
	static boolean q23(int x[])
	{
		float avg = 0;
		for (int n : x)
			avg += n;
		avg /= x.length;

		int balance = 0;
		// Balance is appended and decremented
		// If there is an equal amount of below average
		// and above it will be 0
		for (int n : x)
			if (n > avg)
				balance++;
			else if (n < avg)
				balance--;
		
		if (balance == 0)
			return true;
		return false;
	}

	static void q24a(int x[])
	{
		// -1 since at x.length-1 there will be no
		// next index. + no cancerous Java exceptions.
		for (int i = 0; i < x.length-1; i++)
			if (i < x[i+1])
				System.out.println(i);
	}

	static void q24b()
	{
		final int TWENTY = 20;
		int x[] = new int[TWENTY];
		for (int i = 0; i < x.length; i++)
			x[i] = (int)(Math.random()*0x7FFFFFFF);
		q24a(x);
	}

	static void q25a(int x[], int y)
	{
		boolean odd = y%2==0;
		for (int i = odd?1:0; i < x.length; i++)
			System.out.println(x[i]);
	}

	static void q25b(int x[])
	{

	}
	
	static void q26a(int x[])
	{
		for (int i = 0; i < x.length; i++)
		{
			x[i] += (int)(x[i]*0.1F);
			if (x[i] > 100)
				x[i] = 100;
		}
	}
	
	static int []q26b(int num)
	{
		int ret[] = new int[num];
		Scanner s = new Scanner(System.in);
		for (; num > 0; num--)
		{
			// This may be reversed. but. nobody said it must not be ;)
			ret[num] = s.nextInt();
		}
		return ret;
	}

	static void q26c(int []x)
	{
		for (int y : x)
			System.out.println(y);
	}

	static void q26d()
	{
		int num = new Scanner(System.in).nextInt();
		int []x = q26b(num);
		q26a(x);
		q26c(x);
	}

	// I didn't even read the questions and I knew
	// I needed a function. My IQ is measured in
	// horse power.
	static int getlast(int x)
	{
		while (x > 9)
			x/=10;
		return x%10;
	}

	static int getfirst(int x)
	{
		return x%10;
	}

	static boolean q27(int x[])
	{
		for (int i = 1; i < x.length; i++)
		{
			if (getlast(x[i]) != getfirst(x[i-1]))
				return false;
		}
		return true;
	}

	// This is annoyingly innefficient.
	static void q28(int x[])
	{
		// Save of the number that appred the most
		int mostnum = 0;
		int mostcount = 0;

		for (int i = 0; i < x.length; i++)
		{
			// Current counted number
			int nownum = x[i];
			int nowcount = 0;
				
			for (int j = 0; j < x.length; j++)
				if (x[j] == nownum)
					nowcount++;
			
			if (nowcount > mostcount)
			{
				mostnum = nownum;
				mostcount = nowcount;
			}
		}

		System.out.println("Number is "+mostnum+" Appears "+mostcount+" times.");
	}

	public static void main(String args[])
	{
		System.out.println(q27(new int[]{544, 41, 1, 1462, 243, 32}));
	}
}