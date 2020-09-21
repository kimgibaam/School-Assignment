package com.example.embedded;

public class ChatInfo
{
    private String name;
    private String message;
    private boolean isMe;

    public void setName(String name)
    {
        this.name = name;
    }

    public void setMessage(String message)
    {
        this.message = message;
    }

    public void setIsMe(boolean isMe)
    {
        this.isMe = isMe;
    }

    public String getName()
    {
        return name;
    }

    public String getMessage()
    {
        return message;
    }

    public boolean getIsMe()
    {
        return isMe;
    }

}
