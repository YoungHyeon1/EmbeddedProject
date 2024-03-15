package com.example.embeddedapp;

public class BluetoothItems {
    private String title;
    private String mac_address;

    public BluetoothItems() {
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getMacAddress()
    {
        return mac_address;
    }
    public void SetMacAddress(String mac_address){
        this.mac_address = mac_address;
    }
}
