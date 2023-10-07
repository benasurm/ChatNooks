#pragma once
#include <windows.h>

class KeyEvents
{
	public:
		bool SelectRoomOpearation(Messages &mess_obj, KEY_EVENT_RECORD &ker, 
			RoomOption &room_option)
		{
			if(ker.uChar.UnicodeChar == '\u0031')
			{
				mess_obj.CreatingChatRoomMessage(1);
				room_option = server;
				return true;
			}
			else if(ker.uChar.UnicodeChar == '\u0032')
			{
				mess_obj.CreatingChatRoomMessage(2);
				room_option = client;
				return true;
			}
			return false;
		}
};