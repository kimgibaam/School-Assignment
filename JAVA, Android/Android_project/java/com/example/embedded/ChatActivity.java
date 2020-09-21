package com.example.embedded;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.Toast;

import com.google.firebase.database.ChildEventListener;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

public class ChatActivity extends AppCompatActivity {

    private ListView lv_chating;
    private EditText et_send;
    private Button btn_send;

    private ChatAdapter chatAdapter;
    private ArrayList<ChatInfo> arrayList = new ArrayList<>();

    private String str_room_name;
    private String str_user_name;
    private String member;
    private String number;

    private DatabaseReference reference;
    private DatabaseReference root;

    private String key;
    private String chat_user;
    private String chat_message;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_chat);

        et_send = (EditText)findViewById(R.id.et_send);
        lv_chating = (ListView)findViewById(R.id.lv_chating);
        btn_send = (Button)findViewById(R.id.btn_send);

        str_room_name = getIntent().getExtras().get("room_name").toString();
        str_user_name = getIntent().getExtras().get("user_name").toString();
        member = getIntent().getExtras().get("member").toString();  // 참가자 리스트
        number = getIntent().getExtras().get("number").toString();  // 참가자 수

        reference = FirebaseDatabase.getInstance().getReference().child(str_room_name);
        root = FirebaseDatabase.getInstance().getReference().getRoot().child(str_room_name);

        setTitle(str_room_name + " 채팅방");

        chatAdapter = new ChatAdapter();

        lv_chating.setAdapter(chatAdapter);
        lv_chating.setTranscriptMode(ListView.TRANSCRIPT_MODE_ALWAYS_SCROLL);

        btn_send.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                key = reference.push().getKey();
                // reference.updateChildren(map);  이거하면 채팅방 리스트도 업뎃됨

                DatabaseReference root = reference.child(key);

                Map<String,Object> objectMap = new HashMap<String, Object>();
                objectMap.put("name",str_user_name);
                objectMap.put("message",et_send.getText().toString());

                root.updateChildren(objectMap);
                et_send.setText("");

            }
        });

        reference.addChildEventListener(new ChildEventListener() {
            @Override
            public void onChildAdded(@NonNull DataSnapshot dataSnapshot, @Nullable String s) {
                chatConversation(dataSnapshot);
            }

            @Override
            public void onChildChanged(@NonNull DataSnapshot dataSnapshot, @Nullable String s) {
                chatConversation(dataSnapshot);
            }

            @Override
            public void onChildRemoved(@NonNull DataSnapshot dataSnapshot) {

            }

            @Override
            public void onChildMoved(@NonNull DataSnapshot dataSnapshot, @Nullable String s) {

            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        });

    }

    //액션버튼 메뉴 액션바에 집어 넣기
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu, menu);
        return true;
    }

    //액션버튼을 클릭했을때의 동작
    @Override
    public boolean onOptionsItemSelected(MenuItem item){
        int id = item.getItemId();
        //or switch문을 이용하면 될듯 하다.
        if (id == R.id.action_custom) {

            Intent intent = new Intent(getApplicationContext(), Custom.class);
            startActivity(intent);

            return true;
        }

        return super.onOptionsItemSelected(item);
    }




    private void chatConversation(DataSnapshot dataSnapshot)
    {
        Iterator i = dataSnapshot.getChildren().iterator();

        while(i.hasNext())
        {
            ChatInfo chatInfo = new ChatInfo();

            chat_message = (String)((DataSnapshot)i.next()).getValue();  // 데이터 받기
            chat_user = (String)((DataSnapshot)i.next()).getValue();

            if(chat_user.equals(str_user_name))  // 자신이 입력한 메시지는 다르게 처리함
                chatInfo.setIsMe(true);
            else
                chatInfo.setIsMe(false);

            chatInfo.setName(chat_user);
            chatInfo.setMessage(chat_message);

            arrayList.add(chatInfo);
        }

        chatAdapter.addItem(arrayList);

        chatAdapter.notifyDataSetChanged();
    }

    public void onDestroy()  // 액티비티 사라질때
    {
        getOut();    // 데이터 베이스 갱신

        super.onDestroy();
    }

    public void onPause()   // 앱이 강제종료 가능 상태가 되었을때
    {
        getOut();    // 데이터 베이스 갱신함

        super.onPause();
    }

    public void getOut()   // 채팅방 나갈때 참가자 리스트에서 자기 이름 뺌
    {
        int num = Integer.parseInt(number);
        num -= 1;

        if(num == 0)
            member = member.replace(str_user_name,"");
        else
            member = member.replace(", " + str_user_name,"");

        Map<String,Object> objectMap = new HashMap<String, Object>();
        objectMap.put("population", member + "#population");
        objectMap.put("number", num + "#number");

        root.updateChildren(objectMap);

    }


}

