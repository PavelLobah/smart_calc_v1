//Подключаем заголовочные файлы gtk
#include <gtk/gtk.h>

#include "smart_calc.h"
//  максимальные значения системы координат (ширина окна)
#define X_AXIS 600
#define Y_AXIS 600
//  центр системы координат
#define X_0 X_AXIS / 2
#define Y_0 Y_AXIS / 2

#define STEP_AXIS X_AXIS / 20

void build_coord_system(cairo_t* cr);
gboolean on_draw(GtkWidget* widget, cairo_t* cr, gpointer data);

char main_string[256] = {};
double val_x;
double val_x_max = 10;
double val_y_max = 10;
int i = 0;

// Слот выхода из программы
G_MODULE_EXPORT void onExit(GtkWidget* w) { gtk_main_quit(); }
G_MODULE_EXPORT void destroy(GtkWidget* w) { gtk_main_quit(); }
G_MODULE_EXPORT void pushButton_show(GtkButton* btn, gpointer label1) {
  char res[256] = {0};
  char postfix_expr[256] = {0};
  double result;
  Stack_d stack_d;
  stack_d.size_d = 0;
  if (main_string[0] == '\0') {
    gtk_label_set_text(label1, "INCORRECT DATA");
  } else {
    result = calc(to_postfix(main_string, postfix_expr), stack_d, val_x);
    sprintf(res, "%.7f", result);
    delete_null(res);
    gtk_label_set_text(label1, res);
  }
}
G_MODULE_EXPORT void pushButton_graph(GtkButton* btn) {
  GtkBuilder* ui_builder;
  GError* err = NULL;
  ui_builder = gtk_builder_new();
  if (!gtk_builder_add_from_file(ui_builder, "./calculator.glade", &err)) {
    g_critical("Не вышло загрузить файл с UI : %s", err->message);
    g_error_free(err);
  }
  GtkWidget* graph =
      GTK_WIDGET(gtk_builder_get_object(ui_builder, "graph_window"));
  GtkWidget* drawarea =
      GTK_WIDGET(gtk_builder_get_object(ui_builder, "drawarea"));
  gtk_window_set_default_size(GTK_WINDOW(graph), X_AXIS, Y_AXIS);
  g_signal_connect(drawarea, "draw", G_CALLBACK(on_draw), NULL);
  gtk_window_set_title(GTK_WINDOW(graph), "Graph");
  gtk_window_set_position(GTK_WINDOW(graph), GTK_WIN_POS_CENTER);
  gtk_builder_connect_signals(ui_builder, NULL);
  gtk_widget_show_all(graph);
}
G_MODULE_EXPORT void pushButton_clean(GtkButton* btn, gpointer label1) {
  strncpy(main_string, "", sizeof(main_string));
  i = 0;
  gtk_label_set_text(label1, main_string);
}
G_MODULE_EXPORT void pushButton_backspase(GtkButton* btn, gpointer label1) {
  main_string[strlen(main_string) - 1] = '\0';
  i--;
  gtk_label_set_text(label1, main_string);
}
G_MODULE_EXPORT void pushButton(GtkButton* btn, gpointer label1) {
  //  char str[1];
  char str[2];
  strcpy(str, gtk_button_get_label(GTK_BUTTON(btn)));
  main_string[i++] = str[0];
  gtk_label_set_text(label1, main_string);
}
G_MODULE_EXPORT void pushButton_long(GtkButton* btn, gpointer label1) {
  char str[6];
  strcpy(str, gtk_button_get_label(GTK_BUTTON(btn)));
  for (size_t j = 0; j < strlen(str); j++) {
    main_string[i++] = str[j];
  }
  main_string[i++] = '(';
  gtk_label_set_text(label1, main_string);
}
G_MODULE_EXPORT void pushButton_dot(GtkButton* btn, gpointer label1) {
  char str[1];
  strcpy(str, gtk_button_get_label(GTK_BUTTON(btn)));
  if ((main_string[strlen(main_string) - 1] >= '0') &&
      (main_string[strlen(main_string) - 1] <= '9')) {
    main_string[i++] = str[0];
  }
  gtk_label_set_text(label1, main_string);
}

G_MODULE_EXPORT void push_0(GtkButton* btn, gpointer label1) {
  main_string[i++] = '0';
  gtk_label_set_text(label1, main_string);
}

G_MODULE_EXPORT void on_x_value_insert_text(GtkEntry* entry) {
  char str_val_x[11] = {0};
  sprintf(str_val_x, "%s", gtk_entry_get_text(entry));
  if (str_val_x[0] == '-') {
    val_x = stod(str_val_x) * (-1);
  } else {
    val_x = stod(str_val_x);
  }
}

G_MODULE_EXPORT void on_y_max_value_insert_text(GtkEntry* entry) {
  char str_val_y_max[11] = {0};
  sprintf(str_val_y_max, "%s", gtk_entry_get_text(entry));
  if (str_val_y_max[0] == '-') {
    val_y_max = stod(str_val_y_max) * (-1);
  } else {
    val_y_max = stod(str_val_y_max);
  }
}

G_MODULE_EXPORT void on_x_max_value_insert_text(GtkEntry* entry) {
  char str_val_x_max[11] = {0};
  sprintf(str_val_x_max, "%s", gtk_entry_get_text(entry));
  if (str_val_x_max[0] == '-') {
    val_x_max = stod(str_val_x_max) * (-1);
  } else {
    val_x_max = stod(str_val_x_max);
  }
}
gboolean on_draw(GtkWidget* widget, cairo_t* cr, gpointer data) {
  build_coord_system(cr);
  double x, y;
  char postfix_expr[256] = {0};
  Stack_d stack_d;
  stack_d.size_d = 0;
  to_postfix(main_string, postfix_expr);
  cairo_set_source_rgb(cr, 1, .6, 0);
  double div_scale_x = X_0 / val_x_max;
  double div_scale_y = Y_0 / val_y_max;
  for (double i = -val_x_max * 0.95; i < val_x_max * 0.95; i += 0.01) {
    x = i;
    y = calc(postfix_expr, stack_d, x);
    if (y != INFINITY || y != -INFINITY || y != NAN) {
      cairo_move_to(cr, x * div_scale_x + X_0, Y_0 - y * div_scale_y);
      cairo_show_text(cr, ".");
    }
  }
  cairo_stroke(cr);
  return TRUE;
}

void build_coord_system(cairo_t* cr) {
  //  background
  cairo_set_source_rgb(cr, 0.4, 0.4, 0.4);
  cairo_rectangle(cr, 0, 0, X_AXIS, Y_AXIS);
  cairo_fill(cr);

  //  axis
  cairo_set_line_width(cr, 1);
  cairo_set_source_rgb(cr, 0, 0, 0);

  cairo_move_to(cr, X_0, 0);
  cairo_line_to(cr, X_0, Y_AXIS);
  cairo_move_to(cr, X_AXIS, Y_0);
  cairo_line_to(cr, 0, Y_0);

  cairo_stroke(cr);

  //  system of axes
  char buf[11] = {0};
  cairo_set_line_width(cr, 0.5);
  int x = STEP_AXIS, y = 0;
  double size_x = val_x_max / 10;
  double size_y = val_y_max / 10;
  for (; x < X_AXIS;
       x += STEP_AXIS, size_x += val_x_max / 10, size_y += val_y_max / 10) {
    if (x == X_0) continue;
    //  text
    sprintf(buf, "%.1lf", size_x - val_x_max);
    cairo_set_source_rgba(cr, 0, 0, 0, 0.5);
    cairo_move_to(cr, x - 10, Y_0 + 15);
    cairo_show_text(cr, buf);
    sprintf(buf, "%.1lf", val_y_max - size_y);
    cairo_move_to(cr, X_0 - STEP_AXIS, x + 3);
    cairo_show_text(cr, buf);
    //  green cage
    cairo_set_source_rgba(cr, .4, 1, 0, 0.3);
    cairo_move_to(cr, x, y);
    cairo_line_to(cr, x, Y_AXIS);
    cairo_move_to(cr, y, x);
    cairo_line_to(cr, X_AXIS, x);
    cairo_stroke(cr);
    //  marker
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_move_to(cr, x, Y_0 + 5);
    cairo_line_to(cr, x, Y_0 - 5);
    cairo_move_to(cr, X_0 + 5, x);
    cairo_line_to(cr, X_0 - 5, x);
    cairo_stroke(cr);
  }
}

int main(int argc, char* argv[]) {
  gtk_init(&argc, &argv);
  GtkBuilder* ui_builder;
  GError* err = NULL;
  ui_builder = gtk_builder_new();
  if (!gtk_builder_add_from_file(ui_builder, "./calculator.glade", &err)) {
    g_critical("Не вышло загрузить файл с UI : %s", err->message);
    g_error_free(err);
  }
  GtkWidget* window =
      GTK_WIDGET(gtk_builder_get_object(ui_builder, "main_window"));
  gtk_builder_connect_signals(ui_builder, NULL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(window), "SmartCalculator");
  gtk_widget_show_all(window);
  gtk_main();
  return 0;
}