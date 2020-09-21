package hw2;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.util.Calendar;

public class hw2 {

	public static void main (String[] args)
	{
		JFrame frame = new JFrame();
		JPanel clock = new ClockPanel();
		
		frame.getContentPane().add(clock);
		frame.setTitle("������ �ð�");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.pack();
		frame.setVisible(true);
	}
}

class ClockPanel extends JPanel
{
	private JTextField timeField;
	private Timer t;
	
	public ClockPanel()
	{
		timeField = new JTextField(10);  //  5->10
		timeField.setEditable(false);
		timeField.setFont(new Font("sansserif",Font.PLAIN,48));
		add(timeField);
		
		t = new Timer(1000,new ClockListener());
		t.start();
		
	}
	
	private class ClockListener implements ActionListener
	{
		public void actionPerformed(ActionEvent e)
		{
			Calendar time = Calendar.getInstance();
			int hour = time.get(Calendar.HOUR_OF_DAY);
			int min = time.get(Calendar.MINUTE);
			int sec = time.get(Calendar.SECOND);
			
			String ampm = "AM";  // ������ �κ�
			if(hour > 12)
			{
				hour -= 12;
				ampm = "PM";
			}
			
			timeField.setText(ampm + " " +(hour < 10 ? "0" + hour : hour) + ":"
					+ (min < 10? "0" + min: min) + ":" + (sec<10?"0" + sec
							:sec));
		}
	}
}



