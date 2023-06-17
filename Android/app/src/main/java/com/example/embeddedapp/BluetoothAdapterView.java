package com.example.embeddedapp;
import static android.provider.AlarmClock.EXTRA_MESSAGE;

import static androidx.core.content.ContextCompat.startActivity;

import android.bluetooth.BluetoothAdapter;
import android.content.Intent;
import android.content.Context;
import android.graphics.Color;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;

public class BluetoothAdapterView extends BaseAdapter{
    private Context _context;
    private BluetoothAdapter _parent_bluetooth;
    public BluetoothAdapterView(Context context, BluetoothAdapter parent_bluetooth) {
        this._context = context;
        this._parent_bluetooth = parent_bluetooth;
    }
    private ArrayList<BluetoothItems> items = new ArrayList<>();
    @Override
    public int getCount() {
        return items.size();
    }

    @Override
    public Object getItem(int position) {
        return items.get(position);
    }

    @Override
    public long getItemId(int position) {
        return 0;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        Context context= parent.getContext();
        if (convertView == null) {
            LayoutInflater inflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            convertView = inflater.inflate(R.layout.bluetooth_custom, parent, false);
        }

        TextView bluetooth_title = (TextView) convertView.findViewById(R.id.bluetooth_title);
        Button bluetootn_button = (Button) convertView.findViewById(R.id.bluetooth_connect_button);

        BluetoothItems bt_items = (BluetoothItems) getItem(position);
        bluetooth_title.setText(bt_items.getTitle());
        bluetootn_button.setBackgroundColor(Color.parseColor("#F6A333"));
        bluetootn_button.setTag(position);
        bluetootn_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try{
                    System.out.println((int)v.getTag());
                    if (_parent_bluetooth.isDiscovering())
                    {
                        _parent_bluetooth.cancelDiscovery();
                    }
                    Toast.makeText(context, items.get((int)v.getTag()).getMacAddress(), Toast.LENGTH_LONG).show();
                    items.get(v.getTag().hashCode()).getMacAddress();
                    Intent intent = new Intent(_context, BlueToothViews.class);
                    intent.putExtra("mac_address", items.get((int)v.getTag()).getMacAddress());
                    _context.startActivity(intent);
                } catch (SecurityException e){

                }

            }
        });
        return convertView;
    }

    public void addItem(String name, String mac_address) {

        BluetoothItems mItem = new BluetoothItems();

        /* MyItem에 아이템을 setting한다. */
        mItem.setTitle(name);
        mItem.SetMacAddress(mac_address);
        /* mItems에 MyItem을 추가한다. */
        items.add(mItem);

    }

}
