package hw1;

import java.util.*;

public class hw1 {

	public static void main(String[] args) {
		// TODO Auto-generated method stub

		double weight,height;
		
		Scanner scan = new Scanner(System.in);
		System.out.println("������(kg)�� Ű(m)�� �Է��ϼ���");
		
		weight = scan.nextDouble();
		height = scan.nextDouble();
		
		double result = bmi(weight,height);
		
		if(result <15.0)
		{
			System.out.println("������ ��ü��");
		}
		else if(result >= 15.0 && result < 18.5)
		{
			System.out.println("��ü��");
		}
		else if(result >= 18.5 && result < 23.0)
		{
			System.out.println("����");
		}
		else if(result >= 23.0 && result <= 27.5)
		{
			System.out.println("��ü��");
		}
		else if(result > 27.5 && result <= 40)
		{
			System.out.println("��");
		}
		else if(result > 40)
		{
			System.out.println("������ ��");
		}
		
	}
	
        static double bmi(double weight,double height) {
        	
        	double result = weight/(height*height);
        
        	return result;
	}

}
