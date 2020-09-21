package com.example.embedded;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

import com.google.android.gms.auth.api.signin.GoogleSignIn;
import com.google.android.gms.auth.api.signin.GoogleSignInAccount;
import com.google.android.gms.auth.api.signin.GoogleSignInClient;
import com.google.android.gms.auth.api.signin.GoogleSignInOptions;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.api.ApiException;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.AuthCredential;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.auth.GoogleAuthProvider;


public class LoginActivity extends AppCompatActivity implements GoogleApiClient.OnConnectionFailedListener {

    private FirebaseAuth mAuth;
    private GoogleSignInClient mGoogleSignInClient;
    private static final int RC_SIGN_IN = 9001;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        GoogleSignInOptions gso = new GoogleSignInOptions.Builder(GoogleSignInOptions.DEFAULT_SIGN_IN)
                .requestIdToken(getString(R.string.default_web_client_id))
                .requestEmail()
                .build();


        mGoogleSignInClient = GoogleSignIn.getClient(this, gso);

        findViewById(R.id.Google_Button).setOnClickListener(onClickListener); // 클릭시 id 받아옴
        findViewById(R.id.login_button).setOnClickListener(onClickListener);
        findViewById(R.id.sign_up_login).setOnClickListener(onClickListener);

        mAuth = FirebaseAuth.getInstance();

    }

    @Override
    public void onStart() {
        super.onStart();
        // Check if user is signed in (non-null) and update UI accordingly.
        FirebaseUser currentUser = mAuth.getCurrentUser();


    }

    @Override
    public void onBackPressed()      // 뒤로가기버튼 어플종료, 로그아웃은 안됨 !
    {
        super.onBackPressed();
        moveTaskToBack(true);
        android.os.Process.killProcess(android.os.Process.myPid());
        System.exit(1);
    }


    View.OnClickListener onClickListener = new View.OnClickListener() { // 버튼에 따른 로직
        @Override
        public void onClick(View view) {
            switch (view.getId()) {
                case R.id.Google_Button:
                    G_signIn();
                    break;
                case R.id.login_button :
                    signIn();
                    break;
                case R.id.sign_up_login :
                    startSignupActivity();
                    break;
            }
        }
    };


    private void signIn() {           // 일반 회원 로그인

        String email = ((EditText)findViewById(R.id.EditText_email)).getText().toString();   // email,password 값 찾기
        String password = ((EditText)findViewById(R.id.EditText_password)).getText().toString();


        if(email.length() > 0 && password.length() > 0) {  // 기존 유저 로그인
            mAuth.signInWithEmailAndPassword(email, password)
                    .addOnCompleteListener(this, new OnCompleteListener<AuthResult>() {
                        @Override
                        public void onComplete(@NonNull Task<AuthResult> task) {
                            if (task.isSuccessful()) {
                                FirebaseUser user = mAuth.getCurrentUser();
                                startToast("로그인에 성공 하였습니다.");
                                startMainActivity();     // 로그인성공하면 레이아웃 넘어감
                            } else {
                                if (task.getException() != null) {
                                    startToast("이메일 또는 비밀번호가 틀렸습니다.");
                                }
                            }
                        }
                    });
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

    @Override
    public void onConnectionFailed(@NonNull ConnectionResult connectionResult) {

    }

    private void G_signIn() {    // 구글 로그인 창
        Intent signInIntent = mGoogleSignInClient.getSignInIntent();
        startActivityForResult(signInIntent, RC_SIGN_IN);
    }

    @Override               // 구글 계정 파이어베이스로 !
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if (requestCode == RC_SIGN_IN) {
            Task<GoogleSignInAccount> task = GoogleSignIn.getSignedInAccountFromIntent(data);
            try {
                GoogleSignInAccount account = task.getResult(ApiException.class);
                firebaseAuthWithGoogle(account);
            } catch (ApiException e) {
                startToast("구글 인증 실패");

            }
        }
    }

    private void firebaseAuthWithGoogle(GoogleSignInAccount acct) {   // 구글로 로그인

        AuthCredential credential = GoogleAuthProvider.getCredential(acct.getIdToken(), null);
        mAuth.signInWithCredential(credential)
                .addOnCompleteListener(this, new OnCompleteListener<AuthResult>() {
                    @Override
                    public void onComplete(@NonNull Task<AuthResult> task) {
                        if (task.isSuccessful()) {
                            FirebaseUser user = mAuth.getCurrentUser();
                            startToast("로그인에 성공 하였습니다.");
                            startMainActivity();     // 로그인성공하면 레이아웃 넘어감

                        } else {
                            startToast("로그인 실패");
                        }

                    }
                });
    }

    private void startMainActivity()
    {
        Intent intent = new Intent(this,MainActivity.class);
        intent .addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);  // 다시못돌아가게함
        startActivity(intent);
    }

    private void startSignupActivity()
    {
        Intent intent = new Intent(this,SignupActivity.class);
        startActivity(intent);
    }


}

