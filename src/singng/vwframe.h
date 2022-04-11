
#ifndef _vwframe_h
#define _vwframe_h
/* ------------------------------------------------------------------------- */
/* Warning : this file has been generated and should not be modified by hand */
/* ------------------------------------------------------------------------- */
/* Sing Version   : 2.1a.0.26                                                */
/* Production     : C                                                        */
/* Project        : vwframe.xml                                              */
/* Target         : vwframe.h                                                */
/* Identification : 0.0-1177511938-4355.4354                                 */
/* ------------------------------------------------------------------------- */


private struct vwframe_context {
	int	status;
	int	keycode;
	int	focus_item;
	int	focus_items;
	int	window_col;
	int	window_row;
	int	language;
	char * msg_WidgetProperties[8];
	char * hint_WidgetProperties[8];
	int	x_WidgetProperties;
	int	y_WidgetProperties;
	int	w_WidgetProperties;
	int	h_WidgetProperties;
	char * msg_Auto2779[8];
	char * hint_Auto2779[8];
	char * hint_Auto2780[8];
	char * msg_Auto2781[8];
	char * hint_Auto2781[8];
	char * msg_Auto2782[8];
	char * hint_Auto2782[8];
	char * msg_Auto2783[8];
	char * hint_Auto2783[8];
	char * msg_Auto2784[8];
	char * hint_Auto2784[8];
	char * hint_Identity[8];
	char	buffer_Identity[256];
	char * msg_WidgetClass[8];
	char * hint_WidgetClass[8];
	int	value_WidgetClass;
	char * msg_Align[8];
	char * hint_Align[8];
	int	value_Align;
	char * hint_Hint[8];
	char	buffer_Hint[266];
	char * msg_IsLock[8];
	int	trigger_IsLock;
	char * hint_IsLock[8];
	int	value_IsLock;
	char * hint_AtX[8];
	char	buffer_AtX[8];
	char * hint_AtY[8];
	char	buffer_AtY[8];
	char * hint_Width[8];
	char	buffer_Width[8];
	char * hint_Height[8];
	char	buffer_Height[8];
	char * msg_TextFont[8];
	char * hint_TextFont[8];
	int	value_TextFont;
	char * hint_PayLoad[8];
	char	buffer_PayLoad[256];
	char * msg_IswResize[8];
	int	trigger_IswResize;
	char * hint_IswResize[8];
	int	value_IswResize;
	char * msg_IshResize[8];
	int	trigger_IshResize;
	char * hint_IshResize[8];
	int	value_IshResize;
	char * hint_Auto2785[8];
	char * hint_TextColour[8];
	char	buffer_TextColour[7];
	char * hint_Background[8];
	char	buffer_Background[7];
	char * msg_auto5[8];
	char * hint_auto5[8];
	char * msg_IsBold[8];
	int	trigger_IsBold;
	char * hint_IsBold[8];
	int	value_IsBold;
	char * msg_IsUnderline[8];
	int	trigger_IsUnderline;
	char * hint_IsUnderline[8];
	int	value_IsUnderline;
	char * msg_IsShadow[8];
	int	trigger_IsShadow;
	char * hint_IsShadow[8];
	int	value_IsShadow;
	char * msg_auto6[8];
	char * hint_auto6[8];
	char * msg_auto7[8];
	char * hint_auto7[8];
	char * msg_auto8[8];
	char * hint_auto8[8];
	char * msg_auto64[8];
	char * hint_auto64[8];
	char * hint_StyleSheet[8];
	char	buffer_StyleSheet[256];
	char * msg_auto73[8];
	char * hint_auto73[8];
	char * msg_auto67[8];
	char * hint_auto67[8];
	char * msg_fgSelect[8];
	int	trigger_fgSelect;
	char * hint_fgSelect[8];
	char * msg_auto70[8];
	char * hint_auto70[8];
	char * msg_Auto2786[8];
	char * hint_Auto2786[8];
	char * msg_Auto2787[8];
	char * hint_Auto2787[8];
	char * hint_Auto2788[8];
	char * hint_Auto2789[8];
	char * hint_Auto2790[8];
	char * msg_auto138[8];
	char * hint_auto138[8];
	char * msg_auto77[8];
	char * hint_auto77[8];
	char * msg_auto80[8];
	char * hint_auto80[8];
	char	signature[1];
	};
void	vwframe_position_at(struct vwframe_context * _Context,int x,int y);
void	vwframe_SetVars(struct vwframe_context * _Context);
void	vwframe_GetVars(struct vwframe_context * _Context);

#endif /* _vwframe_h */
