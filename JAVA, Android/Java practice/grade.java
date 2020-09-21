import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;

public class grade {

	public static void main(String[] args) {
		// TODO Auto-generated method stub

		JFrame frame = new JFrame("Grade");
		
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		MakePanel panel = new MakePanel();
		
		frame.getContentPane().add(panel);
		frame.pack();
		frame.setVisible(true);
	}
}

class MakePanel extends JPanel
{
	private JLabel label,label2,gradeLabel,myNum;
	private JTextField myGrade;
	private Integer[] grades = {80,95,95,87,70,0};
	private int input;
	
	public MakePanel()
	{
		myGrade = new JTextField(10);
		label = new JLabel("본인의 점수를 입력하세요");
		label2 = new JLabel("현재 학생들의 점수 : ");
		myNum = new JLabel("");
		
		add(label2);
		for(int i = 0 ; i < 5 ; i++)
		{
			gradeLabel = new JLabel("" + grades[i]);
			add(gradeLabel);
		}
		
		
		setPreferredSize(new Dimension(300,150));
		
		myGrade.addActionListener(new DistListener());
		
		add(label);
		add(myGrade);
		add(myNum);
		
	}
	
	private class DistListener implements ActionListener
	{
		public void actionPerformed(ActionEvent event)
		{
			StringBuffer temp = new StringBuffer("");
			
			input = Integer.parseInt(myGrade.getText());
			
			grades[5] = input;
			Arrays.sort(grades,Collections.reverseOrder()); 
			
			for(int i = 0 ; i < 6 ; i++)
			{
				if(i >= 1 && grades[i] == grades[i -1])
				{
					temp.append((i) + "등 : " + grades[i] + " ");
				}
				else
				{
					temp.append((i + 1) + "등 : " + grades[i] + " ");
				}
			}
			
			myNum.setText(temp.toString());
			
		}
	}
}






