package hw1;

import java.util.*;

public class hw2 {

	public static void main(String[] args) {
		// TODO Auto-generated method stub

		Random random = new Random();
		int dice;
		int a,b,sum1,sum2;
		Scanner scan = new Scanner(System.in);
		
		System.out.println("0이 아닌 주사위의 면 수를 입력하세요(n)");
		dice = scan.nextInt();
		
		while(dice == 0)
		{
			System.out.println("0이 아닌 주사위의 면 수를 입력하세요(n)");
			dice = scan.nextInt();
		}
		a = random.nextInt(dice)+1;
		b = random.nextInt(dice)+1;
		sum1 = a+b;
		a = random.nextInt(dice)+1;
		b = random.nextInt(dice)+1;
		sum2 = a+b;
		
		System.out.printf("유저1의 눈의 합 : %d%n유저2의 눈의 합 : %d%n",sum1, sum2);
		
		if(sum1 > sum2) System.out.println("유저1 승리");
		else if(sum1 < sum2) System.out.println("유저2 승리");
		else System.out.println("비겼습니다");
		
	}

}
