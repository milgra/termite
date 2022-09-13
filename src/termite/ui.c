//  Created by Milan Toth on 2018. 01. 19.

#ifndef ui_h
#define ui_h

void ui_init(void);
void ui_free(void);

#endif

#if __INCLUDE_LEVEL__ == 0

#include "bus.c"
#include "defaults.c"
#include "renderer.c"
#include "str_util.c"
#include "voxel.c"
#include "voxelutil.c"
#include "zc_cstring.c"
#include "zc_string.c"
#include "zc_util2.c"
#include "zc_vector.c"
#include <float.h>

#define WTH 800.0
#define HTH 1000.0
#define MARGIN 10.0

typedef enum
{

    UISTATE_MAIN,
    UISTATE_GRID_A,
    UISTATE_GRID_B,
    UISTATE_HELP,
    UISTATE_START,
    UISTATE_GAME,
    UISTATE_LOST,
    UISTATE_WON

} uistate;

struct _ui
{

    uistate state;

    vec_t* names;
    vec_t* voxels;
    vec_t* buttons;

    voxel_t* sldbtn;
    voxel_t* sldback;

    v2_t screen;
    m4_t mat_proj;
    m4_t mat_pers;

    float base_scale;
    float base_angle;
    float curr_angle;

} ui = {0};

// generates button and adds it to ui containers

void ui_add_button(char* label, float x, float y, float width, float height, uint32_t textcolor, uint32_t backcolor, float pixelsize)
{

    char*  string;
    vec_t* button;

    string = cstr_new_cstring(label);

    button = voxelutil_button(string, x, y, width, height, pixelsize, textcolor, backcolor);

    VADD(ui.names, string);

    VADD(ui.buttons, button->data[button->length - 1]);

    vec_add_in_vector(ui.voxels, button);

    REL(string);
    REL(button);
}

void ui_generate_background()
{

    uint32_t color = 0;

    voxel_t* voxel = HEAP(((voxel_t){

	.model = {
	    WTH / -2.0,
	    0.0,
	    3.0,
	    WTH,
	    HTH,
	    5.0},

	.colors = {[0 ... 7] = color}

    }));

    VADD(ui.voxels, voxel);

    REL(voxel);
}

void ui_generate_header()
{

    v2_t itemasize = {0};
    v2_t itembsize = {0};
    v2_t itemcsize = {0};

    itemasize = pixeltext_calcsize("TERMITE", 29.0);

    ui_add_button("TERMITE", itemasize.x / -2.0 - MARGIN, -100.0, itemasize.x + 2 * MARGIN, itemasize.y + 2 * MARGIN, defaults.textcolor_a, 0, 29.0);

    itembsize = pixeltext_calcsize("BY MILAN TOTH", 8.0);

    ui_add_button("BY MILAN TOTH", itemasize.x / 2.0 - itembsize.x - MARGIN, -130.0 - itemasize.y, itembsize.x + 2 * MARGIN, itembsize.y + 2 * MARGIN, defaults.textcolor_a, defaults.buttoncolor_a, 8.0);

    itemcsize = pixeltext_calcsize(TERMITE_VERSION, 5.0);

    ui_add_button(TERMITE_VERSION, (itemasize.x - itemcsize.x) / -2.0 + 4 * MARGIN,
#if defined(IOS) || defined(ANDROID)
		  -HTH + itemcsize.y + 2 * MARGIN,
#else
		  -HTH + 4 * itemcsize.y,
#endif
		  itemcsize.x + 2 * MARGIN,
		  itemcsize.y + 2 * MARGIN,
		  defaults.textcolor_a,
		  defaults.buttoncolor_a,
		  5.0);
}

void ui_generate_menu(vec_t* items)
{

    ui_generate_background();

    str_t* item;
    char*  label;

    v2_t itemsize = {0};
    v2_t position = {0.0, -650.0 + (items->length * 72.0) / 2.0};

    for (int index = 0;
	 index < items->length;
	 index++)
    {

	item  = items->data[index];
	label = str_new_cstring(item);

	itemsize = pixeltext_calcsize(label, 10.0 * defaults.text_scale);

	ui_add_button(label, itemsize.x / -2.0 - MARGIN, position.y, itemsize.x + 2 * MARGIN, itemsize.y + 2 * MARGIN, defaults.textcolor_a, defaults.buttoncolor_a, 10.0 * defaults.text_scale);

	position.y -= itemsize.y + MARGIN;

	REL(label);
    }
}

char* helptext[] = {
    "SELECT ANT : DRAW SQUARE",
    "SELECT ALL : DOUBLE TAP",
    "TARGET ANT : SINGLE TAP",
    "YOU ARE THE GREEN COLONY",
    "DESTROY ALIEN COLONIES",
    "START GAME"};

char* wontext[] = {
    "YOU WON",
    "CONTINUE"};

char* losttext[] = {
    "YOU LOST",
    "RESTART"};

char* starttext[] = {
    "START"};

void ui_generate_info(char** items, int count)
{

    ui_generate_background();

    char* label;

    float size = 10.0;
    if (count > 4) size = 8.0;
    v2_t itemsize = {0};
    v2_t position = {0.0, HTH / -2.0 + (count * 72.0) / 2.0};

    for (int index = 0;
	 index < count;
	 index++)
    {

	label = items[index];

	itemsize = pixeltext_calcsize(label, size);

	position.x = itemsize.x / -2.0;

	if (index == 3 || index == count - 1) position.y -= itemsize.y + 12.0; // gaps between groups
	if (count > 3 && index < 3) position.x = -355.0;                       // helptext left align

	ui_add_button(label, position.x, position.y, itemsize.x + 2 * MARGIN, itemsize.y + 2 * MARGIN, defaults.textcolor_a, defaults.buttoncolor_a, size);

	position.y -= itemsize.y + 12.0;
    }
}

void ui_generate_toolbar()
{

    v2_t     itemsize;
    uint32_t color;

    float size_spd = 400.0;
    float size_mnu = 200.0;
    float size_hth = 60.0 * defaults.text_scale;

    itemsize = pixeltext_calcsize("S", 10.0 * defaults.text_scale);
    itemsize.y += 10.0 * 2; // add margin

    ui_generate_background();

    color = defaults.buttoncolor_a;

    ui_add_button("SPEED", -(size_spd + size_mnu) / 2.0 - 2.0, -HTH + size_hth * defaults.text_scale, size_spd, size_hth, defaults.textcolor_a, defaults.buttoncolor_a, 6.0);

    ui.sldback = ui.voxels->data[ui.voxels->length - 1];

    ui_add_button("MENU", (size_spd + size_mnu) / 2.0 - size_mnu + 2.0, -HTH + size_hth * defaults.text_scale, size_mnu, size_hth, defaults.textcolor_a, defaults.buttoncolor_a, 6.0);

    color = 0xFFFFFF22;

    voxel_t* voxel = HEAP(((voxel_t){

	.model = {-270.0, -HTH + size_hth * defaults.text_scale, 16.0, 70.0, size_hth, 5.0},

	.colors = {[0 ... 7] = color}

    }));

    ui.sldbtn = voxel;

    VADD(ui.voxels, voxel);

    REL(voxel);
}

void ui_generate_level_selector(vec_t* voxels, vec_t* names, vec_t* buttons, int startindex)
{

    v2_t  itemsize = {0};
    int   counter;
    char  text[10];
    float x, y;
    float size     = 120.0;
    float start    = -220.0;
    float textsize = 10.0;

    if (defaults.text_scale > 1.0) textsize = 12.0;

    ui_generate_background();

    counter = startindex + 1;

    for (int row = 0;
	 row < 4;
	 row++)
    {

	for (int col = 0;
	     col < 4;
	     col++)
	{

	    snprintf(text, 10, "%.2i", counter++);

	    x = start + col * size;
	    y = start + row * size * -1;

	    uint32_t color = defaults.buttoncolor_a;
	    if (counter - 1 <= defaults.highest_level) color = defaults.selectcolor;

	    itemsize = pixeltext_calcsize(text, textsize);

	    ui_add_button(text, x, y, itemsize.x + 2 * MARGIN, itemsize.y + 2 * MARGIN, defaults.textcolor_a, color, textsize);
	}
    }

    itemsize = pixeltext_calcsize(">", textsize);

    if (startindex == 0)
    {

	ui_add_button(">", start + 4.0 * size, start - 3.0 * size, itemsize.x + 2 * MARGIN, itemsize.y + 2 * MARGIN, defaults.textcolor_a, defaults.buttoncolor_a, textsize);
    }
    else
    {

	ui_add_button("<", start - size + 10.0, start - 3.0 * size, itemsize.x + 2 * MARGIN, itemsize.y + 2 * MARGIN, defaults.textcolor_a, defaults.buttoncolor_a, textsize);
    }

    itemsize = pixeltext_calcsize("RESET STATE", 10.0);

    ui_add_button("RESET STATE", -itemsize.x / 2.0 - MARGIN, -6.0 * size, itemsize.x + 2 * MARGIN, itemsize.y + 2 * MARGIN, defaults.textcolor_a, defaults.buttoncolor_a, 10.0);
}

void ui_updateui()
{

    renderdata_t data =
	{
	    .projection = ui.mat_proj,
	    .level      = 3,
	    .voxels     = ui.voxels};

    bus_notify("SCN", "UPLOADVOXELS", &data);
}

void ui_updatescale()
{

    float scale = 0.5;

    float wthratio   = ui.screen.x / WTH;
    float testheight = HTH * wthratio;

    float hthratio  = ui.screen.y / HTH;
    float testwidth = WTH * hthratio;

    if (testwidth <= ui.screen.x) scale = hthratio;
    else if (testheight <= ui.screen.y) scale = wthratio;

    ui.base_scale = scale;
}

void ui_updateperspective()
{

    float camera_fov_y = M_PI / 4.0;
    float camera_eye_z = (ui.screen.y / 2.0) / (tanf(camera_fov_y / 2.0));

    float min = camera_eye_z - (HTH / 2.0) * ui.base_scale;
    float max = camera_eye_z + (HTH / 2.0) * ui.base_scale;
    if (min < 10.0) min = 10.0;

    m4_t pers = m4_defaultperspective(camera_fov_y, ui.screen.x / ui.screen.y, min, max);

    m4_t trans = m4_defaulttranslation(0, 0, -camera_eye_z);

    ui.mat_pers = m4_multiply(pers, trans);
}

void ui_updateprojection()
{

    ui.curr_angle += (ui.base_angle - ui.curr_angle) / 20.0;

    m4_t angle_matrix = m4_defaultrotation(ui.curr_angle, 0.0, 0.0);

    m4_t scale_matrix = m4_defaultscale(ui.base_scale, ui.base_scale, ui.base_scale);

    m4_t trans_matrix;

    if (ui.state != UISTATE_GAME)
    {

	// translate contents to vertical center

	trans_matrix = m4_defaulttranslation(0.0, HTH / 2.0, 0.0);
    }
    else
    {

	// translate contents to bottom

	float from_top = (ui.screen.y - HTH * ui.base_scale);
	float from_cen = (ui.screen.y / 2.0 - from_top) / ui.base_scale;

	if (defaults.depth_on == 1) from_cen += 70.0;

	trans_matrix = m4_defaulttranslation(0.0, from_cen, 0.0);
    }

    ui.mat_proj = m4_multiply(ui.mat_pers, angle_matrix);

    ui.mat_proj = m4_multiply(ui.mat_proj, scale_matrix);

    ui.mat_proj = m4_multiply(ui.mat_proj, trans_matrix);

    ui_updateui();
}

void ui_generate_main_menu(char* menuitem)
{

    vec_t* mainitems = VNEW();

    if (menuitem == NULL)
    {
	VADDR(mainitems, str_frombytes("PLAY"));
	VADDR(mainitems, str_frombytes("OPTIONS"));
	if (defaults.prices_arrived == 1) VADDR(mainitems, str_frombytes("DONATE"));
#if !defined(IOS) && !defined(ANDROID)
	VADDR(mainitems, str_frombytes("EXIT"));
#endif
    }
    else if (strcmp(menuitem, "PLAY") == 0)
    {
	VADDR(mainitems, str_frombytes("TOURNAMENT"));
	VADDR(mainitems, str_frombytes("RANDOM"));
	VADDR(mainitems, str_frombytes("BACK"));
    }
    else if (strcmp(menuitem, "OPTIONS") == 0)
    {
	VADDR(mainitems, str_frombytes("2D/3D"));
	VADDR(mainitems, str_frombytes("COLOR"));
	VADDR(mainitems, str_frombytes("EFFECTS"));
#if !defined(IOS) && !defined(ANDROID)
	VADDR(mainitems, str_frombytes("FULLSCREEN"));
#endif
	VADDR(mainitems, str_frombytes("FEEDBACK"));
	VADDR(mainitems, str_frombytes("BACK"));
    }
    else if (strcmp(menuitem, "DONATE") == 0)
    {
	for (int index = 0; index < 3; index++)
	{
	    VADDR(mainitems, str_frombytes(defaults.prices[index]));
	}
	VADDR(mainitems, str_frombytes("BACK"));
    }
    else if (strcmp(menuitem, "THANKS") == 0)
    {
	VADDR(mainitems, str_frombytes("THANK YOU!!!"));
	VADDR(mainitems, str_frombytes("BACK"));
    }
    else if (strcmp(menuitem, "PLSGIVE") == 0)
    {
	VADDR(mainitems, str_frombytes("TO PLAY"));
	VADDR(mainitems, str_frombytes("LEVEL 22-32"));
	VADDR(mainitems, str_frombytes("PLEASE DONATE"));
	VADDR(mainitems, str_frombytes(""));
	VADDR(mainitems, str_frombytes("BACK"));
    }

    ui_generate_header();
    ui_generate_menu(mainitems);

    REL(mainitems);
}

void ui_generate_grid(int from)
{

    ui_generate_level_selector(ui.voxels, ui.names, ui.buttons, from);
}

void ui_changestate(uistate state, char* menuitem)
{

    vec_reset(ui.voxels);
    vec_reset(ui.names);
    vec_reset(ui.buttons);

    if (state == UISTATE_MAIN) ui_generate_main_menu(menuitem);
    else if (state == UISTATE_GRID_A) ui_generate_grid(0);
    else if (state == UISTATE_GRID_B) ui_generate_grid(16);
    else if (state == UISTATE_HELP) ui_generate_info(helptext, 6);
    else if (state == UISTATE_START) ui_generate_info(starttext, 1);
    else if (state == UISTATE_GAME) ui_generate_toolbar();
    else if (state == UISTATE_LOST) ui_generate_info(losttext, 2);
    else if (state == UISTATE_WON) ui_generate_info(wontext, 2);

    ui.state = state;

    ui_updatescale();
    ui_updateperspective();
    ui_updateprojection();
}

void ui_loadlevel(int level)
{

    if (level < 0) level = 0;
    if (level > 32) level = 32;

    defaults.current_level = level;

    bus_notify("UI", "LOAD", &defaults.current_level);
}

void ui_handle_gridbutton(char* name)
{

    if (name[0] == '>') ui_changestate(UISTATE_GRID_B, NULL);
    else if (name[0] == '<') ui_changestate(UISTATE_GRID_A, NULL);
    else if (strcmp(name, "RESET STATE") == 0)
    {
	defaults.highest_level = 1;
	defaults_save();
	ui_changestate(UISTATE_GRID_A, NULL);
    }
    else
    {

	int level = atoi(name);
	if (level == 1) ui_changestate(UISTATE_HELP, NULL);
	else
	{
	    // check level availability

	    if (level <= defaults.highest_level)
	    {
		// ask for donation
		if (defaults.donation_arrived == 0 && level > 21)
		{

		    ui_changestate(UISTATE_MAIN, "PLSGIVE");
		}
		else
		{

		    ui_loadlevel(level);
		    if (level == 32) ui_changestate(UISTATE_GAME, NULL);
		    else ui_changestate(UISTATE_START, NULL);
		}
	    }
	}
    }
}

void ui_handle_mainbutton(char* name)
{

    if (strcmp(name, "BY MILAN TOTH") == 0) bus_notify("UI", "HOMEPAGE", NULL);

    else if (strcmp(name, "PLAY") == 0) ui_changestate(UISTATE_MAIN, "PLAY");
    else if (strcmp(name, "OPTIONS") == 0) ui_changestate(UISTATE_MAIN, "OPTIONS");
    else if (strcmp(name, "DONATE") == 0)
    {
#ifdef RASPBERRY
	bus_notify("UI", "DONATE", name);
#else
	ui_changestate(UISTATE_MAIN, "DONATE");
#endif
    }
    else if (strcmp(name, "EXIT") == 0) bus_notify("UI", "EXIT", NULL);

    else if (strcmp(name, "TOURNAMENT") == 0) ui_changestate(UISTATE_GRID_A, NULL);
    else if (strcmp(name, "RANDOM") == 0)
    {

	ui_changestate(UISTATE_START, NULL);
	defaults.current_level = -1;
	bus_notify("UI", "LOAD", &defaults.current_level);
    }
    else if (strcmp(name, "BACK") == 0) ui_changestate(UISTATE_MAIN, NULL);

    else if (strcmp(name, "FEEDBACK") == 0) bus_notify("UI", "FEEDBACK", NULL);
    else if (strcmp(name, "FULLSCREEN") == 0) bus_notify("UI", "FULLSCREEN", NULL);
    else if (strcmp(name, "2D/3D") == 0)
    {

	defaults.depth_on = 1 - defaults.depth_on;
	defaults_save();

	ui.base_angle = defaults.depth_on == 1 ? -M_PI / 6 : 0;

	bus_notify("UI", "2D/3D", NULL);
    }
    else if (strcmp(name, "COLOR") == 0)
    {

	if (defaults.dark_on == 1) defaults_setbrighttheme();
	else defaults_setdarktheme();
	defaults_save();

	defaults.current_level = 0;
	bus_notify("UI", "LOAD", &defaults.current_level);

	ui_changestate(ui.state, "OPTIONS");
    }
    else if (strcmp(name, "EFFECTS") == 0)
    {

	defaults.effects_level += 1;
	if (defaults.effects_level == 3) defaults.effects_level = 0;
	defaults_save();

	bus_notify("UI", "EFFECTS_LEVEL", NULL);

	ui_updateprojection();
    }
    else if (defaults.prices_arrived == 1)
    {
	for (int index = 0; index < 3; index++)
	{
	    if (strcmp(defaults.prices[index], name) == 0)
	    {
		bus_notify("UI", "DONATE", name);
		ui_changestate(UISTATE_MAIN, "THANKS");
	    }
	}
    }
}

void ui_handle_otherbutton(char* name)
{

    if (strcmp(name, "MENU") == 0) // from game state
    {
	ui_changestate(UISTATE_MAIN, NULL);
	ui_loadlevel(0);
    }
    else if (strcmp(name, "START") == 0) // from start state
    {
	ui_changestate(UISTATE_GAME, NULL);
	bus_notify("UI", "START", NULL);
    }
    else if (strcmp(name, "CONTINUE") == 0) // from game_won state
    {
	if (defaults.current_level == -1) ui_changestate(UISTATE_MAIN, NULL);
	else
	{

	    ui_loadlevel(defaults.current_level + 1);

	    if (defaults.donation_arrived == 0 && defaults.current_level > 21)
	    {
		// ask for donation
		ui_changestate(UISTATE_MAIN, "PLSGIVE");
	    }
	    else if (defaults.current_level == 32) ui_changestate(UISTATE_GAME, NULL);
	    else ui_changestate(UISTATE_START, NULL);
	}
    }
    else if (strcmp(name, "RESTART") == 0) // from game_lost state
    {
	if (defaults.current_level == -1)
	{
	    ui_changestate(UISTATE_START, NULL);
	    bus_notify("UI", "LOAD", &defaults.current_level);
	}
	else
	{
	    ui_loadlevel(defaults.current_level);
	    ui_changestate(UISTATE_START, "START GAME");
	}
    }
    else if (strcmp(name, "START GAME") == 0) // from help state
    {
	ui_loadlevel(1);
	ui_changestate(UISTATE_START, "START GAME");
    }
}

void ui_buttonpressed(char* name)
{

    if (ui.state == UISTATE_MAIN) ui_handle_mainbutton(name);
    else if (ui.state == UISTATE_GRID_A) ui_handle_gridbutton(name);
    else if (ui.state == UISTATE_GRID_B) ui_handle_gridbutton(name);
    else ui_handle_otherbutton(name);
}

void ui_buttonmoved(char* name, v2_t local)
{

    if (strcmp(name, "SPEED") == 0)
    {
	float half = ui.sldbtn->model.w / 2.0;

	if (local.x + half > ui.sldback->model.w) local.x = ui.sldback->model.w - half;
	if (local.x < half) local.x = half;

	ui.sldbtn->model.x = ui.sldback->model.x + local.x - half;
	ui_updateui();

	float ratio = (local.x - half) / (ui.sldback->model.w - ui.sldbtn->model.w);

	bus_notify("UI", "SETSPEED", &ratio);
    }
}

char* ui_getpushedbutton(void* data, v2_t* local)
{

    v2_t screenpt = *(v2_t*) data;

    vec_t* items = ui.buttons;
    vec_t* names = ui.names;

    v4_t front = v4_init(screenpt.x, ui.screen.y - screenpt.y, -1.0, 0.0);

    v4_t back = v4_init(screenpt.x, ui.screen.y - screenpt.y, 0.0, 0.0);

    v3_t frontprj = m4_screen_to_world_coords(ui.mat_proj, front, ui.screen.x, ui.screen.y);

    v3_t backprj = m4_screen_to_world_coords(ui.mat_proj, back, ui.screen.x, ui.screen.y);

    for (int index = 0;
	 index < items->length;
	 index++)
    {

	voxel_t* cube = items->data[index];

	v3_t is = voxel_intersect(cube, frontprj, backprj, "03");

	if (is.x != FLT_MAX)
	{

	    local->x = is.x - cube->model.x;
	    local->y = is.y - cube->model.y;

	    return names->data[index];
	}
    }

    return NULL;
}

void ui_onmessage(const char* name, void* data)
{

    if (strcmp(name, "UPDATE") == 0)
    {

	if (fabs(ui.curr_angle - ui.base_angle) > 0.0001)
	{
	    ui_updateprojection();
	}
    }
    else if (strcmp(name, "TOUCHDOWN") == 0)
    {

	v2_t local;

	char* name = ui_getpushedbutton(data, &local);

	if (name != NULL) ui_buttonpressed(name);
    }
    else if (strcmp(name, "TOUCHMOVE") == 0 || strcmp(name, "TOUCHUP") == 0)
    {

	v2_t local;

	char* name = ui_getpushedbutton(data, &local);

	if (name != NULL) ui_buttonmoved(name, local);
    }
    else if (strcmp(name, "RESIZE") == 0)
    {

	v2_t* newdimensions = data;

	ui.screen.x = newdimensions->x;
	ui.screen.y = newdimensions->y;

	ui_updatescale();
	ui_updateperspective();
	ui_updateprojection();

	if (ui.state == UISTATE_MAIN) ui_loadlevel(0);
    }
    else if (strcmp(name, "SHOWDONATION") == 0)
    {

	if (ui.state == UISTATE_MAIN)
	{
	    ui_changestate(UISTATE_MAIN, NULL);
	}
    }
    else if (strcmp(name, "LOST") == 0)
    {
	ui_changestate(UISTATE_LOST, NULL);
    }
    else if (strcmp(name, "WON") == 0)
    {

	if (defaults.highest_level < defaults.current_level + 1)
	{
	    defaults.highest_level = defaults.current_level + 1;
	    defaults_save();
	}

	ui_changestate(UISTATE_WON, NULL);
    }
}

void ui_init()
{

    bus_subscribe("CTL", ui_onmessage);
    bus_subscribe("SCN", ui_onmessage);

    ui.buttons = VNEW();
    ui.voxels  = VNEW();
    ui.names   = VNEW();

    ui.base_angle = defaults.depth_on == 1 ? -M_PI / 6 : 0;
    ui.curr_angle = ui.base_angle + 0.001;

    ui_changestate(UISTATE_MAIN, NULL);
}

void ui_free()
{

    bus_unsubscribe("CTL", ui_onmessage);
    bus_unsubscribe("SCN", ui_onmessage);

    REL(ui.buttons);
    REL(ui.voxels);
    REL(ui.names);

    ui.sldbtn  = NULL;
    ui.sldback = NULL;
}

#endif
