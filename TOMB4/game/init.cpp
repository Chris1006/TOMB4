#include "../tomb4/pch.h"
#include "init.h"
#include "objects.h"
#include "../specific/function_stubs.h"
#include "control.h"
#include "door.h"
#include "items.h"
#include "traps.h"

void InitialiseMapper(short item_number)
{
	items[item_number].mesh_bits = 0xFFFFFFFD;	//hide laser
}

void InitialiseLightningConductor(short item_number)
{
	ITEM_INFO* item;
	long pack;

	item = &items[item_number];

	if (item->trigger_flags == 2)
	{
		pack = 0;

		for (int i = 0; i < level_items; i++)
		{
			if (items[i].object_number == ANIMATING8)
			{
				item->item_flags[2] |= i << (pack != 0 ? 8 : 0);
				pack++;
			}
		}
	}
}

void InitialiseDoor(short item_number)
{
	ITEM_INFO* item;
	ROOM_INFO* r;
	ROOM_INFO* b;
	DOOR_DATA* door;
	long dx, dy;
	short two_room, box_number, room_number;

	item = &items[item_number];
	door = (DOOR_DATA*)game_malloc(sizeof(DOOR_DATA));
	item->data = door;
	door->Opened = 0;
	dx = 0;
	dy = 0;

	if (!item->pos.y_rot)
		dx = -1;
	else if (item->pos.y_rot == -32768)
		dx = 1;
	else if (item->pos.y_rot == 16384)
		dy = -1;
	else
		dy = 1;

	r = &room[item->room_number];
	door->d1.floor = &r->floor[(((item->pos.z_pos - r->z) >> 10) + dx) + (((item->pos.x_pos - r->x) >> 10) + dy) * r->x_size];
	room_number = GetDoor(door->d1.floor);

	if (room_number == 255)
		box_number = door->d1.floor->box;
	else
	{
		b = &room[room_number];
		box_number = b->floor[(((item->pos.z_pos - b->z) >> 10) + dx) + (((item->pos.x_pos - b->x) >> 10) + dy) * b->x_size].box;
	}

	door->d1.block = (boxes[box_number].overlap_index & 0x8000) ? box_number : 2047;
	memcpy(&door->d1.data, door->d1.floor, sizeof(FLOOR_INFO));

	if (r->flipped_room == -1)
		door->d1flip.floor = 0;
	else
	{
		r = &room[r->flipped_room];
		door->d1flip.floor = &r->floor[(((item->pos.z_pos - r->z) >> 10) + dx) + (((item->pos.x_pos - r->x) >> 10) + dy) * r->x_size];
		room_number = GetDoor(door->d1flip.floor);

		if (room_number == 255)
			box_number = door->d1flip.floor->box;
		else
		{
			b = &room[room_number];
			box_number = b->floor[(((item->pos.z_pos - b->z) >> 10) + dx) + (((item->pos.x_pos - b->x) >> 10) + dy) * b->x_size].box;
		}

		door->d1flip.block = (boxes[box_number].overlap_index & 0x8000) ? box_number : 2047;
		memcpy(&door->d1flip.data, door->d1flip.floor, sizeof(FLOOR_INFO));
	}

	two_room = GetDoor(door->d1.floor);
	ShutThatDoor(&door->d1);
	ShutThatDoor(&door->d1flip);

	if (two_room == 255)
	{
		door->d2.floor = 0;
		door->d2flip.floor = 0;
	}
	else
	{
		r = &room[two_room];
		door->d2.floor = &r->floor[((item->pos.z_pos - r->z) >> 10) + ((item->pos.x_pos - r->x) >> 10) * r->x_size];
		room_number = GetDoor(door->d2.floor);

		if (room_number == 255)
			box_number = door->d2.floor->box;
		else
		{
			b = &room[room_number];
			box_number = b->floor[((item->pos.z_pos - b->z) >> 10) + ((item->pos.x_pos - b->x) >> 10) * b->x_size].box;
		}

		door->d2.block = (boxes[box_number].overlap_index & 0x8000) ? box_number : 2047;
		memcpy(&door->d2.data, door->d2.floor, sizeof(FLOOR_INFO));

		if (r->flipped_room == -1)
			door->d2flip.floor = 0;
		else
		{
			r = &room[r->flipped_room];
			door->d2flip.floor = &r->floor[((item->pos.z_pos - r->z) >> 10) + ((item->pos.x_pos - r->x) >> 10) * r->x_size];
			room_number = GetDoor(door->d2flip.floor);

			if (room_number == 255)
				box_number = door->d2flip.floor->box;
			else
			{
				b = &room[room_number];
				box_number = b->floor[((item->pos.z_pos - b->z) >> 10) + ((item->pos.x_pos - b->x) >> 10) * b->x_size].box;
			}

			door->d2flip.block = (boxes[box_number].overlap_index & 0x8000) ? box_number : 2047;
			memcpy(&door->d2flip.data, door->d2flip.floor, sizeof(FLOOR_INFO));
		}

		ShutThatDoor(&door->d2);
		ShutThatDoor(&door->d2flip);
		room_number = item->room_number;
		ItemNewRoom(item_number, two_room);
		item->room_number = room_number;
	}
}

void InitialiseTrapDoor(short item_number)
{
	CloseTrapDoor(&items[item_number]);
}

void InitialiseFallingBlock2(short item_number)
{
	items[item_number].mesh_bits = 1;
}

void InitialiseFlameEmitter(short item_number)
{
	ITEM_INFO* item;

	item = &items[item_number];

	if (item->trigger_flags < 0)
	{
		item->item_flags[0] = (GetRandomControl() & 0x3F) + 90;
		item->item_flags[2] = 256;

		if ((-item->trigger_flags & 7) == 7)
		{
			if (!item->pos.y_rot)
				item->pos.z_pos += 512;
			else if (item->pos.y_rot == 0x4000)
				item->pos.x_pos += 512;
			else if (item->pos.y_rot == -0x8000)
				item->pos.z_pos -= 512;
			else if (item->pos.y_rot == -0x4000)
				item->pos.x_pos -= 512;
		}
	}
}

void InitialiseFlameEmitter2(short item_number)
{
	ITEM_INFO* item;

	item = &items[item_number];
	item->pos.y_pos -= 64;

	if (item->trigger_flags != 123)
	{
		if (!item->pos.y_rot)
		{
			if (item->trigger_flags == 2)
				item->pos.z_pos += 80;
			else
				item->pos.z_pos += 256;
		}
		else if (item->pos.y_rot == 0x4000)
		{
			if (item->trigger_flags == 2)
				item->pos.x_pos += 80;
			else
				item->pos.x_pos += 256;
		}
		else if (item->pos.y_rot == -0x8000)
		{
			if (item->trigger_flags == 2)
				item->pos.z_pos -= 80;
			else
				item->pos.z_pos -= 256;
		}
		else if (item->pos.y_rot == -0x4000)
		{
			if (item->trigger_flags == 2)
				item->pos.x_pos -= 80;
			else
				item->pos.x_pos -= 256;
		}
	}
}

void InitialiseFlameEmitter3(short item_number)
{
	ITEM_INFO* item;
	ITEM_INFO* item2;

	item = &items[item_number];

	if (item->trigger_flags < 3)
		return;

	for (int i = 0; i < level_items; i++)
	{
		item2 = &items[i];

		if (item2->object_number == ANIMATING3)
		{
			if (item2->trigger_flags == item->trigger_flags)
				item->item_flags[2] = i;
			else if (!item2->trigger_flags)
				item->item_flags[3] = i;
		}
	}
}

void InitialiseJobySpike(short item_number)
{
	ITEM_INFO* item;
	FLOOR_INFO* floor;
	long h, c;
	short room_number;

	item = &items[item_number];
	item->pos.y_rot = short(GetRandomControl() << 10);
	item->item_flags[2] = GetRandomControl() & 1;
	room_number = item->room_number;
	floor = GetFloor(item->pos.x_pos, item->pos.y_pos, item->pos.z_pos, &room_number);
	h = GetHeight(floor, item->pos.x_pos, item->pos.y_pos, item->pos.z_pos);
	c = GetCeiling(floor, item->pos.x_pos, item->pos.y_pos, item->pos.z_pos);
	item->item_flags[3] = short((4096 * (h - c)) / 3328);
}

void InitialiseTwoBlockPlatform(short item_number)
{
	ITEM_INFO* item;

	item = &items[item_number];
	item->item_flags[0] = (short)item->pos.y_pos;
	item->item_flags[1] = 1;
}

void InitialiseSlicerDicer(short item_number)
{
	ITEM_INFO* item;

	item = &items[item_number];
	item->pos.x_pos += 512 * phd_sin(item->pos.y_rot + 0x4000) >> 14;
	item->pos.z_pos += 512 * phd_cos(item->pos.y_rot + 0x4000) >> 14;
	item->item_flags[3] = 50;
	item->item_flags[0] = short(item->pos.x_pos >> 8);
	item->item_flags[1] = short((item->pos.y_pos - 4608) >> 8);
	item->item_flags[2] = short(item->pos.z_pos >> 8);
}

void InitialiseScaledSpike(short item_number)
{
	ITEM_INFO* item;
	short xzrots[8];

	item = &items[item_number];
	xzrots[0] = -0x8000;
	xzrots[1] = -0x6000;
	xzrots[2] = -0x4000;
	xzrots[3] = -0x2000;
	xzrots[4] = 0;
	xzrots[5] = 0x2000;
	xzrots[6] = 0x4000;
	xzrots[7] = 0x6000;
	item->status = ITEM_INVISIBLE;

	if (item->trigger_flags & 8)
	{
		item->pos.x_rot = xzrots[item->trigger_flags & 7];
		item->pos.y_rot = 0x4000;
		item->pos.z_pos -= SPxzoffs[item->trigger_flags & 7];
	}
	else
	{
		item->pos.z_rot = xzrots[item->trigger_flags & 7];
		item->pos.x_pos += SPxzoffs[item->trigger_flags & 7];
	}

	item->item_flags[0] = 1024;
	item->item_flags[2] = 0;
	item->pos.y_pos += SPyoffs[item->trigger_flags & 7];
}

void InitialiseRaisingBlock(short item_number)
{
	ITEM_INFO* item;
	FLOOR_INFO* floor;
	short room_num;

	item = &items[item_number];
	room_num = item->room_number;
	floor = GetFloor(item->pos.x_pos, item->pos.y_pos, item->pos.z_pos, &room_num);
	boxes[floor->box].overlap_index &= 0xBF;

	if (item->object_number == EXPANDING_PLATFORM)
	{
		if (!item->pos.y_rot)
			item->pos.z_pos += 511;
		else if (item->pos.y_rot == 0x4000)
			item->pos.x_pos += 511;
		else if (item->pos.y_rot == -0x8000)
			item->pos.z_pos -= 511;
		else if (item->pos.y_rot == -0x4000)
			item->pos.x_pos -= 511;
	}

	if (item->trigger_flags < 0)
	{
		item->flags |= IFL_CODEBITS;
		AddActiveItem(item_number);
		item->status = ITEM_ACTIVE;
	}
}

void InitialiseBurningFloor(short item_number)
{
	items[item_number].required_anim_state = 127;
}

void InitialiseSethBlade(short item_number)
{
	ITEM_INFO* item;

	item = &items[item_number];
	item->anim_number = objects[SETH_BLADE].anim_index + 1;
	item->frame_number = anims[item->anim_number].frame_base;
	item->current_anim_state = 2;
	item->goal_anim_state = 2;
	item->item_flags[2] = ABS(item->trigger_flags);
}

void inject_init(bool replace)
{
	INJECT(0x004537D0, InitialiseMapper, replace);
	INJECT(0x00453800, InitialiseLightningConductor, replace);
	INJECT(0x00453A30, InitialiseDoor, replace);
	INJECT(0x00453070, InitialiseTrapDoor, replace);
	INJECT(0x004530A0, InitialiseFallingBlock2, replace);
	INJECT(0x004530D0, InitialiseFlameEmitter, replace);
	INJECT(0x00453170, InitialiseFlameEmitter2, replace);
	INJECT(0x00453230, InitialiseFlameEmitter3, replace);
	INJECT(0x004532A0, InitialiseJobySpike, replace);
	INJECT(0x00453340, InitialiseTwoBlockPlatform, replace);
	INJECT(0x00453370, InitialiseSlicerDicer, replace);
	INJECT(0x00453400, InitialiseScaledSpike, replace);
	INJECT(0x004534E0, InitialiseRaisingBlock, replace);
	INJECT(0x004535C0, InitialiseBurningFloor, replace);
	INJECT(0x004535F0, InitialiseSethBlade, replace);
}
