


#include <gtk/gtk.h>

#include <stdio.h>
#include <string.h>


#define  MYDEMO_VERSION     "v1.0.2" 

#define GENERAL_BUF_SIZE   256


char * WidgetNames[] = { 
				/*** DISPLAY WIDGETS ****/
			"GtkLabel","GtkAccelLabel","GtkSpinner",
			"GtkStatusBar","GtkLevelBar","GtkProgressBar","GtkInfoBar",
			"GtkScrollBar","GtkImage","GtkSeparator","GtkTextView",
			"GtkScale", "GtkScale",  /** 11 **/
				/*** BUTTONS ****/
			"GtkButton","GtkCheckButton","GtkToggleButton",
			"GtkLinkButton","GtkMenuButton","GtkLockButton","GtkSpinButton",
			"GtkColorButton","GtkSwitch","GtkComboBox","GtkComboBoxText",
			"GtkFontButton","GtkAppChooserButton","GtkVolumeButton",
			"GtkRadioButton","GtkFileChooserButton",
				/**   Entries   ****/
			"GtkEntry", "GtkSearchEntry" };

const int NumWidgetNames = (sizeof(WidgetNames)/sizeof(char*));
GtkAdjustment * g_Adjustment = NULL;


GtkWidget * allWidgets[60];
GtkWidget * allLabels[60];

const gchar* START_MESSAGE = "START SPINNER";
const gchar* STOP_MESSAGE = "STOP SPINNER";
const gchar* CHECK_LABEL = "Check Button Label";
const gchar* TOGGLE_LABEL = "Toggle = ";


static void BasicScaleCallback( GtkRange * gr, GtkScrollType gsType, gdouble value, gpointer user_data )
{
	GtkAdjustment * adj;
	gdouble lower, upper;
	gdouble srange;
	gdouble pos;
	static double testpos = 1.0;

	adj = gtk_range_get_adjustment( GTK_RANGE(gr) );
	lower = gtk_adjustment_get_lower( adj );
	upper = gtk_adjustment_get_upper( adj );
	srange = upper - lower;
	pos = gtk_adjustment_get_value( adj );
	pos = pos - lower;
	if ( user_data == allWidgets[5] )
		gtk_progress_bar_set_fraction( GTK_PROGRESS_BAR( user_data ), 1-( pos / srange ));
	else if ( user_data == allWidgets[4] )
		{
		testpos = testpos + 1.0;
		if ( testpos > 198.0 )
			testpos = 1;
			
		gtk_level_bar_set_value( GTK_LEVEL_BAR( user_data ), testpos );
		}
}

static void ToggleButtonCallback( GtkToggleButton* cb, gpointer userdata )
{
	gboolean gState = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(cb) );
	char buff[60];
	sprintf(buff,"%s %s", TOGGLE_LABEL,(gchar*) (gState ? "TRUE" : "FALSE") );
	if ( gState )
		gtk_button_set_label( GTK_BUTTON(cb), "TRUE=ON" );
	else
		gtk_button_set_label( GTK_BUTTON(cb), "FALSE=OFF" );

}
static void CheckButtonCallback( GtkCheckButton* cb, gpointer userdata )
{
	gboolean gState = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(cb) );
	if ( gState )
		gtk_button_set_label( GTK_BUTTON(cb), "TRUE=ON" );
	else
		gtk_button_set_label( GTK_BUTTON(cb), "FALSE=OFF" );

}

static void BasicScrollbarCallback( GtkAdjustment * adj,gpointer data )
{
	gdouble value = gtk_adjustment_get_value( adj );
	char buf[100];
	sprintf(buf,"Scrollbar set to %05.2lf", value );
	gtk_label_set_label( GTK_LABEL(data), buf );

}


static void BasicButtonCallback( GtkWidget * widget, gpointer data )
{
	gchar * ptrToState = (gchar*) gtk_button_get_label( GTK_BUTTON(widget));

	if ( strcasecmp( ptrToState, START_MESSAGE ) == 0 )
		{
		gtk_spinner_start( GTK_SPINNER(allWidgets[2]) );
		gtk_button_set_label( GTK_BUTTON(widget), STOP_MESSAGE );
		}
	else if ( strcasecmp( ptrToState, STOP_MESSAGE ) == 0 )
		{
		gtk_spinner_stop( GTK_SPINNER(allWidgets[2]) );
		gtk_button_set_label( GTK_BUTTON(widget), START_MESSAGE );
		}
	else
		{
		fprintf(stderr,"Unexpected state on the spinner start/stop button :  '%s'\n", ptrToState );
		gtk_button_set_label( GTK_BUTTON(widget), STOP_MESSAGE );
		}

}

GtkWidget * MakeADisplayWidget( char * widgetName )
{
	GtkWidget * retval = NULL;
	GtkAdjustment * adjustment;
	GtkTextBuffer * gtb;

	char GeneralString[GENERAL_BUF_SIZE];
	guint tmpUInt;
	gchar TmpString[GENERAL_BUF_SIZE];
	static int gtkScaleInt = 0;


	fprintf( stderr,"BEGIN Creating %s\n", widgetName);

	if ( strcasecmp( widgetName,"GtkLabel" ) == 0 )   /** 0 **/
		{
		sprintf( GeneralString,"Label : '%s'", widgetName );
		retval = gtk_label_new( GeneralString );
		}
	else if ( strcasecmp( widgetName,"GtkAccelLabel" ) == 0 ) /** 1 **/
		{
		retval = gtk_accel_label_new( widgetName );

			/***
			*** This is for later, it is an implemenation of a label for
			*** menus.   
			*****/
			/****
		GtkWidget * save_item;
		GtkAccelGroup * accel_group;
		accel_group = gtk_accel_group_new();
		save_item = gtk_menu_item_new_with_label("save");
		gtk_widget_show( save_item);
			gtk_container_add( GTK_CONTAINER(menu), save_item);

		gtk_widget_add_accelerator( save_item,"activate", accel_group,
						GDK_KEY_s, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
			***/
		}
	else if ( strcasecmp( widgetName,"GtkSpinner" ) == 0 ) /** 2 **/
		{
		retval = gtk_spinner_new();
		}
	else if ( strcasecmp( widgetName,"GtkStatusBar" ) == 0 ) /** 3 **/
		{
		retval = gtk_statusbar_new();
		strcpy( TmpString, "CONTEXT_ID_1" );
		tmpUInt = gtk_statusbar_get_context_id(GTK_STATUSBAR(retval), TmpString );
		gtk_statusbar_push( GTK_STATUSBAR(retval), tmpUInt, "Hello : "  );
		}
	else if ( strcasecmp( widgetName,"GtkLevelBar" ) == 0 ) /** 4 **/
		{
		retval = gtk_level_bar_new_for_interval( 0.0, 200.0 );
		gtk_level_bar_set_value( GTK_LEVEL_BAR(retval), 50.0 );
		}
	else if ( strcasecmp( widgetName,"GtkProgressBar" ) == 0 ) /** 5 **/
		{
		retval = gtk_progress_bar_new();
		gtk_progress_bar_set_fraction( GTK_PROGRESS_BAR(retval), 0.25 );
		}
	else if ( strcasecmp( widgetName,"GtkInfoBar" ) == 0 ) /** 6 **/
		{
		fprintf( stderr,"BEGIN GTK_INFO_BAR\n");
//		retval = gtk_info_bar_new_with_buttons( "ONE" , "TWO");
		}
	else if ( strcasecmp( widgetName,"GtkScrollBar" ) == 0 ) /** 7 **/
		{
		adjustment = gtk_adjustment_new( 50.0, 0.0, 100.0, 1.0, 1.0, 0 );
		retval = gtk_scrollbar_new( GTK_ORIENTATION_HORIZONTAL, adjustment );
		g_signal_connect( adjustment, "value_changed", 
				G_CALLBACK(BasicScrollbarCallback), allLabels[7] );
		}
	else if ( strcasecmp( widgetName,"GtkImage" ) == 0 ) /** 8 **/
		{
		retval = gtk_image_new_from_file("AnIcon.png");
		}
	else if ( strcasecmp( widgetName,"GtkSeparator" ) == 0 ) /*** 9 ****/
		{
		retval = gtk_separator_new( GTK_ORIENTATION_HORIZONTAL );
		}
	else if ( strcasecmp( widgetName,"GtkTextView" ) == 0 ) /*** 10 ***/
		{
#define TEXT_BUFFER_CONTENTS "Hello\nWorld!!"
		gtb = gtk_text_buffer_new( NULL );
		gtk_text_buffer_set_text( gtb, TEXT_BUFFER_CONTENTS, strlen(TEXT_BUFFER_CONTENTS));
		retval = gtk_text_view_new_with_buffer( gtb );
		}
	else if ( strcasecmp( widgetName,"GtkScale" ) == 0 ) /*** 11 ***/
		{
		if ( gtkScaleInt == 0 )
			{
			retval = gtk_scale_new_with_range( GTK_ORIENTATION_HORIZONTAL,0.0,200.0,1.0);
			g_signal_connect( G_OBJECT(retval),"change-value", G_CALLBACK( BasicScaleCallback ), (gpointer) allWidgets[4]  );
			}
		else if ( gtkScaleInt == 1 )
			{
			retval = gtk_scale_new_with_range( GTK_ORIENTATION_HORIZONTAL,100.0,300.0,1.0);
			g_signal_connect( G_OBJECT(retval),"change-value", G_CALLBACK( BasicScaleCallback ), (gpointer) allWidgets[5]  );
			}
		gtkScaleInt++;
		}
/*	else if ( strcasecmp( widgetName,"GtkScaleV" ) == 0 ) *** 12  ***
		{
		retval = gtk_scale_new_with_range( GTK_ORIENTATION_HORIZONTAL,0.0,200.0,1.0);
		g_signal_connect( G_OBJECT(retval),"change-value", G_CALLBACK( BasicScaleCallback ), (gpointer) allWidgets[4]  );
		}
****/
	else if ( strcasecmp( widgetName,"GtkButton" ) == 0 ) /*** 13 ***/
		{
		retval = gtk_button_new_with_label( START_MESSAGE );
		g_signal_connect( G_OBJECT(retval), "clicked", G_CALLBACK( BasicButtonCallback  ), (gpointer) NULL );
		}
	else if ( strcasecmp( widgetName,"GtkCheckButton" ) == 0 ) /*** 14 ***/
		{
		retval = gtk_check_button_new_with_label( CHECK_LABEL );
		g_signal_connect( G_OBJECT(retval), "toggled", G_CALLBACK( CheckButtonCallback  ), (gpointer) NULL  );
		}
	else if ( strcasecmp( widgetName,"GtkToggleButton" ) == 0 ) /*** 15 ***/
		{
		retval = gtk_toggle_button_new_with_label( TOGGLE_LABEL );
		g_signal_connect( G_OBJECT(retval), "toggled", G_CALLBACK( ToggleButtonCallback  ), (gpointer) NULL  );
		}
	else
		fprintf(stderr,"NOT IMPLEMENTED YET... %s\n", widgetName );
	

/****
*****			"GtkButton","GtkCheckButton","GtkToggleButton",
*****			"GtkLinkButton","GtkMenuButton","GtkLockButton","GtkSpinButton",
*****			"GtkColorButton","GtkSwitch","GtkComboBox","GtkComboBoxText",
****/
	fprintf( stderr,"END  Creating  %s  retval = 0x%08x\n",widgetName,(unsigned int)(void*)retval );
	return retval;
}

int main( int argc, char * argv[] )
{
	int retval = 0;

	GtkWidget * window;
	GtkWidget * gtk_grid;
	GtkWidget * label;
	GtkWidget * dWidget;
	gint pos;
	gint left, top, width, height;
	const gint myheight=20;
	const gint mywidth = 40;

	char LabelString[256];
	int i;

	gtk_init( &argc, &argv );

	sprintf(LabelString,"Widget Demos (%d Widgets)", NumWidgetNames );

	window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
	g_signal_connect( G_OBJECT(window), "destroy", 
				G_CALLBACK(gtk_main_quit), NULL );

	gtk_window_set_title( GTK_WINDOW(window), LabelString );



	gtk_grid = gtk_grid_new();
	gtk_container_set_border_width( GTK_CONTAINER(gtk_grid), 10 );
	gtk_container_add( GTK_CONTAINER(window), gtk_grid );

	for( i = 0; i < NumWidgetNames; i++ )
		{
		pos = i;
		gtk_grid_insert_row( (GtkGrid*)gtk_grid, pos );
		}
	for( i = 0; i < 6; i++ )
		{
		pos = i;
		gtk_grid_insert_column((GtkGrid*)gtk_grid, pos );
		}

			/*** LOOP FOR CREATING WIDGETS  ***/
	for( i = 0; i < NumWidgetNames && i < 15; i++ )
		{
		fprintf(stderr,"BEGIN Loop %d  %s\n", i, WidgetNames[i] );
			/*** make and add a label to describe widget***/
		sprintf( LabelString,"%s : ", WidgetNames[i] );
		label = gtk_label_new( LabelString );
		allLabels[i] = label;
		if ( ( i % 6 ) == 0 )
			left = 0;
		else
			left = left + mywidth;
		top = (2*(i/6)) * myheight;
		width = mywidth;
		height = myheight;
		if ( label != NULL )
			gtk_grid_attach( (GtkGrid*)gtk_grid, label, left, top, width, height );
		else
			fprintf(stderr,"ERROR : Label = NULL on loop %d  %s\n",
					i, WidgetNames[i] );

		dWidget = MakeADisplayWidget( WidgetNames[i] );
		allWidgets[i] = dWidget;
		top = top + myheight;
		if ( dWidget != NULL )
			gtk_grid_attach( (GtkGrid*)gtk_grid, dWidget, left, top, width, height );
		else
			fprintf(stderr,"ERROR : dWidget = NULL on loop %d  %s\n",
					i, WidgetNames[i] );
		
		}
	fprintf(stderr,"END Loop\n" );

	/*** Finish up the window and launch it. ***/
	gtk_widget_show_all(window);

	gtk_main();



	return retval;
}

