package hw1;

import java.util.*;

public class hw2 {

	public static void main(String[] args) {
		// TODO Auto-generated method stub

		Random random = new Random();
		int dice;
		int a,b,sum1,sum2;
		Scanner scan = new Scanner(System.in);
		
		System.out.println("0�� �ƴ� �ֻ����� �� ���� �Է��ϼ���(n)");
		dice = scan.nextInt();
		
		while(dice == 0)
		{
			System.out.println("0�� �ƴ� �ֻ����� �� ���� �Է��ϼ���(n)");
			dice = scan.nextInt();
		}
		a = random.nextInt(dice)+1;
		b = random.nextInt(dice)+1;
		sum1 = a+b;
		a = random.nextInt(dice)+1;
		b = random.nextInt(dice)+1;
		sum2 = a+b;
		
		System.out.printf("����1�� ���� �� : %d%n����2�� ���� �� : %d%n",sum1, sum2);
		
		if(sum1 > sum2) System.out.println("����1 �¸�");
		else if(sum1 < sum2) System.out.println("����2 �¸�");
		else System.out.println("�����ϴ�");
		
	}

}
