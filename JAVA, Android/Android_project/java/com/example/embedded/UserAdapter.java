package com.example.embedded;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import java.util.ArrayList;

public class UserAdapter extends BaseAdapter {

    private ArrayList<RoomInfo> listCustom = new ArrayList<>();

    // ListView에 보여질 Item 수
    @Override
    public int getCount() {
        return listCustom.size();
    }

    // 하나의 Item(ImageView 1, TextView 2)
    @Override
    public RoomInfo getItem(int position) {
        return listCustom.get(position);
    }

    // 지정한 위치(position)에 있는 데이터와 관계된 아이템(row)의 ID를 리턴. : 필수 구현
    @Override
    public long getItemId(int position) {
        return position ;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        // 배열에서 position에 위치한 데이터(아이템) 가져오기
        RoomInfo roomInfo = listCustom.get(position);

        // 아이템의 내용을 그림 convertView의 재사용을 위해서 확인
        // null 이면 새로 만들고, null 이 아니면 재사용한다.
        if (convertView == null) {
            convertView = LayoutInflater.from(parent.getContext()).inflate(R.layout.room_list, parent, false);
        }
        // ConvertView에서 Card의 내용을 적용할 뷰 찾기
        TextView title = (TextView) convertView.findViewById(R.id.textView_room_title);
        TextView population = (TextView) convertView.findViewById(R.id.textView_population);
        TextView number =  (TextView) convertView.findViewById(R.id.textView_member_number);
        // 뷰에 값 지정하기.
        title.setText(roomInfo.getTitle());
        population.setText(roomInfo.getPopulation());
        number.setText(roomInfo.getNumber());
        // 완성된 뷰를 반환 ( 화면에 보여주게 된다. )
        return convertView;
    }

    public void addItem(ArrayList arrayList)
    {
        listCustom = arrayList;
    }

}
