package com.example.stepmotor;

import android.app.Activity;
import android.os.Bundle;
import android.widget.RadioGroup;

public class MainActivity extends Activity implements RadioGroup.OnCheckedChangeListener {
	RadioGroup mRadioGroup;
	
	/** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        mRadioGroup = (RadioGroup)findViewById(R.id.RadioGroup01);
        mRadioGroup.setOnCheckedChangeListener(this);
    }

	@Override
	public void onCheckedChanged(RadioGroup group, int checkedId) {
		// TODO Auto-generated method stub
	 	switch(checkedId)
    	{
    	case R.id.RB01 :
    		ReceiveLedValue(1); // right
    		break ;    	
    	case R.id.RB02 :
    		ReceiveLedValue(0); // left
    		break ;    	 	
    	}
		
	}
	
	
	public native String ReceiveLedValue(int x);
	
    static {
        System.loadLibrary("motor-jni");
    }
	
}