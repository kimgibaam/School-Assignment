package com.example.embedded;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

//import FirebaseDatabase

public class MainActivity extends AppCompatActivity {

    private ListView listView;
    private Button btn_create;

    private UserAdapter userAdapter;
    private ArrayList<RoomInfo> arrayList = new ArrayList<>();

    private DatabaseReference reference = FirebaseDatabase.getInstance()
            .getReference().getRoot();

    private DataSnapshot mdataSnapshot;

    private DatabaseReference root;

    private String str_name;
    private String str_room;
    private String member;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        setTitle("채팅방 리스트");

        listView = (ListView) findViewById(R.id.list);     // 리스트뷰
        btn_create = (Button) findViewById(R.id.btn_create);

        userAdapter = new UserAdapter();

        //
        listView.setAdapter(userAdapter);

        btn_create.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                final EditText et_inDialog = new EditText(MainActivity.this);

                final AlertDialog.Builder builder = new AlertDialog.Builder(MainActivity.this);
                builder.setTitle("채팅방 이름 입력");
                builder.setView(et_inDialog);
                builder.setPositiveButton("확인", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {

                        str_room = et_inDialog.getText().toString();

                        root = reference.child(str_room); // 자식 객체 생성

                        Map<String, Object> objectMap = new HashMap<String, Object>();
                        objectMap.put("title", str_room + "#title");
                        objectMap.put("population", "" + "#population");  // 채팅방 참가자
                        objectMap.put("number", 0 + "#number");  // 채팅방 참가자

                        root.updateChildren(objectMap);   // 자식으로 생성

                    }
                });
                builder.setNegativeButton("취소", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        dialog.dismiss();
                    }
                });
                builder.show();
            }
        });



        reference.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                // Set<String> set = new HashSet<String>();
                Iterator i = dataSnapshot.getChildren().iterator();

                mdataSnapshot = dataSnapshot;

                String temp1 = "";     // 임시 문자열변수 두개
                String temp2 = "";

                arrayList.clear();    // 리스트 초기화

                while (i.hasNext()) {
                    // 이 객체 밖에 선언해줘야 리스트별로 나온다.
                    // 반복문 밖에 한개만 선언해주면 마지막 리스트만 여러번 뜸
                    RoomInfo roomInfo = new RoomInfo();   // 데이터 넣을 클래스

                    temp1 = ((DataSnapshot) i.next()).getValue().toString();

                    // getValue 함수로 호출된 값을 원하는 값으로 재조정하는 작업
                    temp2 = temp1.substring(temp1.indexOf("title="), temp1.indexOf("#title"));
                    temp2 = temp2.replace("title=", "");

                    roomInfo.setTitle(temp2);

                   // Toast.makeText(MainActivity.this, temp2, Toast.LENGTH_SHORT).show();

                    temp2 = temp1.substring(temp1.indexOf("population="), temp1.indexOf("#population"));
                    temp2 = temp2.replace("population=", "");

                    roomInfo.setPopulation(temp2);

                    temp2 = temp1.substring(temp1.indexOf("number="), temp1.indexOf("#number"));
                    temp2 = temp2.replace("number=", "");

                    roomInfo.setNumber(temp2);

                    arrayList.add(roomInfo); // 다시 추가
                }

                userAdapter.addItem(arrayList);  // 어댑터에 넣어줌

                userAdapter.notifyDataSetChanged();
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        });

        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                // 레이아웃 -> 텍스뷰에러는 말그대로 뷰를 텍스트뷰로 만들어주면 됨
                str_room = ((TextView) view.findViewById(R.id.textView_room_title)).getText().toString();

                final EditText et_inDialog = new EditText(MainActivity.this);

                final AlertDialog.Builder builder = new AlertDialog.Builder(MainActivity.this);
                builder.setTitle("닉네임 입력");
                builder.setView(et_inDialog);
                builder.setPositiveButton("확인", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {

                        str_name = et_inDialog.getText().toString();

                        Iterator i = mdataSnapshot.getChildren().iterator();

                        root = reference.child(str_room);

                        String temp1 = "";
                        String temp2 = "";
                        int number;  // 참가자 수

                        if(i.hasNext()) {
                            temp1 = ((DataSnapshot) i.next()).getValue().toString();

                            temp2 = temp1.substring(temp1.indexOf("population="), temp1.indexOf("#population"));
                            temp2 = temp2.replace("population=", "");

                            if (temp2.equals(""))
                                temp2 = temp2 + str_name;
                            else
                                temp2 = temp2 + ", " + str_name;

                            member = temp2;   // 현재 참가자 리스트

                            temp2 = temp1.substring(temp1.indexOf("number="), temp1.indexOf("#number"));
                            temp2 = temp2.replace("number=", "");

                            number = Integer.parseInt(temp2);
                            number += 1;   // 참가자 수 증가시키고 입장

                           // Toast.makeText(MainActivity.this, member, Toast.LENGTH_SHORT).show();

                            Map<String, Object> objectMap = new HashMap<String, Object>();
                            objectMap.put("population", member + "#population");  // 채팅방 참가자
                            objectMap.put("number", number + "#number");

                            root.updateChildren(objectMap);   // 자식으로 생성

                            // 액티비티 넘어감
                            Intent intent = new Intent(getApplicationContext(), ChatActivity.class);
                            intent.putExtra("room_name", str_room);
                            intent.putExtra("user_name", str_name);
                            intent.putExtra("member",member);
                            intent.putExtra("number", number);
                            startActivity(intent);
                        }


                    }

                });

                builder.setNegativeButton("취소", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        dialog.dismiss();
                    }
                });
                builder.show();


                //
            }


        });
    }


}


