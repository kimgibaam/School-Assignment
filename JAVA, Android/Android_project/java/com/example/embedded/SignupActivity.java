package com.example.embedded;


import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;

public class SignupActivity extends AppCompatActivity {

    private FirebaseAuth mAuth;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_signup);

        // Initialize Firebase Auth             // 파이어베이스 복붙, 초기화
        mAuth = FirebaseAuth.getInstance();

        findViewById(R.id.sign_up_button).setOnClickListener(onClickListener);  // 클릭
    }


    View.OnClickListener onClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            switch (v.getId()){
                case R.id.sign_up_button :   // 버튼눌렀을때 로직
                    signUp();
                    break;
            }
        }
    };

    private void signUp() {    // 회원가입 기능

        String email = ((EditText)findViewById(R.id.emailEditText)).getText().toString();   // email,password 값 찾기
        String password = ((EditText)findViewById(R.id.passwordEditText)).getText().toString();
        String passwordCheck = ((EditText)findViewById(R.id.passwordCheckEditText)).getText().toString();

        if(email.length() > 0 && password.length() > 0 && passwordCheck.length() > 0) {  // 입력이 되어있어야 실행
            if (password.equals(passwordCheck)) {
                mAuth.createUserWithEmailAndPassword(email, password)         // 파이어베이스 복붙
                        .addOnCompleteListener(this, new OnCompleteListener<AuthResult>() {
                            @Override
                            public void onComplete(@NonNull Task<AuthResult> task) {
                                if (task.isSuccessful()) {
                                    FirebaseUser user = mAuth.getCurrentUser();
                                    startToast("회원가입이 완료 되었습니다.");
                                    startLoginActivity();
                                    //성공 UI 로직
                                } else {
                                    //실패 UI 로직
                                    if (task.getException() != null) {
                                        startToast("이메일 형식이 틀렸습니다.");
                                        //startToast(task.getException().toString());
                                    }
                                }

                            }
                        });
            }
            else
            {
                startToast("비밀번호가 일치하지 않습니다.");
            }
        }
        else
        {
            startToast("이메일 또는 비밀번호를 입력 해주세요.");
        }

    }

    private void startToast(String msg)    // 토스트 생성함수
    {
        Toast.makeText(this, msg,
                Toast.LENGTH_SHORT).show();
    }

    private void startLoginActivity()
    {
        Intent intent = new Intent(this,LoginActivity.class);  // 레이아웃 넘기기
        intent .addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);  // 액티비티 넘기고 다시못돌아가게함
        FirebaseAuth.getInstance().signOut();    // 로그아웃 기능, 지울시 회원가입후 어플종료해도 다시키면 로그인됨
        startActivity(intent);
    }
}