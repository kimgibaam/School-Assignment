package hw1;

import java.util.*;

public class hw1 {

	public static void main(String[] args) {
		// TODO Auto-generated method stub

		double weight,height;
		
		Scanner scan = new Scanner(System.in);
		System.out.println("몸무게(kg)와 키(m)를 입력하세요");
		
		weight = scan.nextDouble();
		height = scan.nextDouble();
		
		double result = bmi(weight,height);
		
		if(result <15.0)
		{
			System.out.println("병적인 저체중");
		}
		else if(result >= 15.0 && result < 18.5)
		{
			System.out.println("저체중");
		}
		else if(result >= 18.5 && result < 23.0)
		{
			System.out.println("정상");
		}
		else if(result >= 23.0 && result <= 27.5)
		{
			System.out.println("과체중");
		}
		else if(result > 27.5 && result <= 40)
		{
			System.out.println("비만");
		}
		else if(result > 40)
		{
			System.out.println("병적인 비만");
		}
		
	}
	
        static double bmi(double weight,double height) {
        	
        	double result = weight/(height*height);
        
        	return result;
	}

}
