package mycard;

import javax.swing.*;
import java.awt.*;

public class mycard {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		JFrame frame = new JFrame("mycard");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
	
		frame.getContentPane().add(new card());
		frame.pack();
		frame.setVisible(true);
		
	}

}

class card extends JPanel
{
	public card()
	{
		setPreferredSize(new Dimension(350,200));
		setBackground(Color.white);
	}
	
	public void paintComponent(Graphics page)
	{
		super.paintComponent(page);
		
		page.setColor(Color.black);
		page.fillOval(25, 120, 10, 10);
		page.fillOval(25, 140, 10, 10);
		page.fillOval(25, 160, 10, 10);
		
		page.drawString("�Ҽ� : �������б� ��ǻ��������Ű��а�", 40, 130);
		page.drawString("�̸� : ����", 40, 150);
		page.drawString("����ó : 01040025965", 40, 170);
		page.drawLine(20, 110, 320, 110);
		
		Image imgae = new ImageIcon("C:/Users/rlqja/Desktop/������.jpg").getImage();
		page.drawImage(imgae,100,0,150,100,this);
	
	}
	
}
