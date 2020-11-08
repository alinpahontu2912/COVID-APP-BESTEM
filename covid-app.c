// gcc `pkg-config gtk+-3.0 --cflags` covid-app.c -o covid-app `pkg-config gtk+-3.0 --libs`
//   --libs gtk+-3.0`
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LEN 42  // numarul de judete + mun Bucuresti

typedef struct {
  char name[30];
  long int population;
  int new_infect;
  float inf_rate;
  int new_tests;
  int code;
} covid_data;
// datele noastre vor fi salvate intr- structura cu aceste campuri

covid_data counties[LEN];
int total_tests;
int total_infect;
float infection_rat;

void print_covid(covid_data county) {
  g_print("----------%s--------\n", county.name);
  g_print("Cod:%d\n", county.code);
  g_print("Populatie: %ld\n", county.population);
  g_print("Infectari in ultimele 24 de ore: %d\n", county.new_infect);
  g_print("Rata pe judet: %.2f‰\n", county.inf_rate);
  g_print("Nr total de teste raportate: %d\n", county.new_tests);
}
// afiseaza datele dintr-o structura

void read_input(covid_data *counties) {
  FILE *input = fopen("input", "r");
  char dummy_inf_rate[4];
  int county_number;
  fscanf(input, "%d", &county_number);
  // consideram mereu 42
  for (int i = 0; i < LEN; i++) {
    fscanf(input, "%s", counties[i].name);
    fscanf(input, "%d", &counties[i].code);
    fscanf(input, "%ld", &counties[i].population);
    fscanf(input, "%d", &counties[i].new_infect);
    fscanf(input, "%s", dummy_inf_rate);
    counties[i].inf_rate = atof(dummy_inf_rate);
    fscanf(input, "%d", &counties[i].new_tests);
  }
  fclose(input);
}
// citim datele din fisierul de input

int show_total_new_test(covid_data *counties) {
  int sum = 0;
  for (int i = 0; i < LEN; i++) {
    sum += counties[i].new_tests;
  }
  return sum;
}
// calculeaza testele noi din intreaga tara

int show_total_new_infect(covid_data *counties) {
  int sum = 0;
  for (int i = 0; i < LEN; i++) {
    sum += counties[i].new_infect;
  }
  return sum;
}
// calculeaza numarul de infectai noi din tara

void print_data(GtkWidget *widget, gpointer data) {
  covid_data *a = data;
  print_covid(*a);
}
// printeaza o structura de tiv covid_data

int cmp_by_infect(const void *a, const void *b) {
  covid_data *ia = (covid_data *)a;
  covid_data *ib = (covid_data *)b;
  return (ib->new_infect - ia->new_infect);
}
// compara dupa numraul de infectari

int cmp_by_inf_rate(const void *a, const void *b) {
  covid_data *ia = (covid_data *)a;
  covid_data *ib = (covid_data *)b;
  return (ib->inf_rate - ia->inf_rate);
}
// compara dupa rata de infectare

static void print_tests(GtkWidget *widget, gpointer data) {
  int *a = data;
  g_print("\nNumarul de teste din ziua anterioara este:%d\n", *a);
}
// afiseaza testele din ziua anterioara

static void print_new_infect(GtkWidget *widget, gpointer data) {
  int *a = data;
  g_print("\nNumarul de infectari din ziua anterioara este:%d\n", *a);
}
// afiseaza infectarile din ziua anterioara

static void print_total_situation(GtkWidget *widget, gpointer data) {
  for (int i = 0; i < LEN; i++) {
    print_covid(counties[i]);
  }
}
// afiseaza datele despre fiecare judet

static void print_infections_situation(GtkWidget *widget, gpointer data) {
  qsort(&counties, LEN, sizeof(covid_data), cmp_by_infect);
  g_print("\n");
  for (int i = 0; i < LEN; i++) {
    g_print("%s - %d\n", counties[i].name, counties[i].new_infect);
  }
  g_print("\n");
}
// afiseaza udetele in functie de numarul de infectari noi

static void print_inf_rate_situation(GtkWidget *widget, gpointer data) {
  qsort(&counties, LEN, sizeof(covid_data), cmp_by_inf_rate);
  g_print("\n");
  for (int i = 0; i < LEN; i++) {
    g_print("%s - %f\n", counties[i].name, counties[i].inf_rate);
  }
  g_print("\n");
}
// afiseaza udetele in functie de rata infectarii din ultimele 24h

static void print_infection_rate(GtkWidget *widget, gpointer data) {
  float *a = data;
  infection_rat = (float)total_infect / total_tests * 100;
  g_print("\nRata de infectare a Romaniei este:%f%% \n", *a);
}
// calculeaza si printeaza rata infectarii a romaniei din ultimele 24%

void print_least_infected(GtkWidget *widget, gpointer data) {
  qsort(&counties, LEN, sizeof(covid_data), cmp_by_infect);
  g_print("\n%s - %d\n", counties[LEN - 1].name, counties[LEN - 1].new_infect);
}
// afiseaza cel mai putin infectat judet

void print_most_infected(GtkWidget *widget, gpointer data) {
  qsort(&counties, LEN, sizeof(covid_data), cmp_by_infect);
  g_print("\n%s - %d\n", counties[0].name, counties[0].new_infect);
}
// afiseaza cel mai infectat judet

void print_min_inf_rate(GtkWidget *widget, gpointer data) {
  qsort(&counties, LEN, sizeof(covid_data), cmp_by_inf_rate);
  g_print("\n%s - %f\n", counties[LEN - 1].name, counties[LEN - 1].inf_rate);
}
// afiseaza judetul cu cea mai mica rata a infectarii

void print_max_inf_rate(GtkWidget *widget, gpointer data) {
  qsort(&counties, LEN, sizeof(covid_data), cmp_by_inf_rate);
  g_print("\n%s - %f\n", counties[0].name, counties[0].inf_rate);
}
// afiseaza judetul cu cea mai mare rata a infectarii

void print_phone_number(GtkWidget *widget, gpointer data) {
  g_print("\nDaca medicul de familie nu este disponibil, sunati la:112 \n");
}
// numar de urgenta

void print_symptoms(GtkWidget *widget, gpointer data) {
  g_print(
      "\nCele mai frecvente simptome sunt: \n-febra, \n-tuse seaca\n-oboseala");
  g_print(
      "\nSimptome mai putin frecvente: dureri musculare, gat umflat,\n diaree, "
      "conjuctivita, dureri de cap, pierderea mirosului si gustului\niritatii "
      "pe piele");
  g_print(
      "\nSimptome severe: -dificultate in respiratie,\n-pierderea orbirii "
      "si/sau a miscarii \n-oboseala\n");
  g_print("Sursa: World Health Organization: who.int\n");
  g_print(
      "Daca nu va simtiti bine si  medicul de familie nu este disponibil, "
      "sunati la:112 \n\n");
}
// afiseaza simpome
void print_tips(GtkWidget *widget, gpointer data) {
  g_print(
      "\nRecomandari:\n-poarta masca de protectie\n-evita zonele "
      "aglomerare\n-dezinfecteaza-te pe maini cat mai des\n-poarta masca "
      "corect(nas si gura)\n-nu reutiliza mastile de unica folosinta\n");
}
// afiseaza sfaturi;

static void activate(GtkApplication *app, gpointer user_data) {
  // toate widgeturile pe care le vom folosi:
  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *button1, *button2, *button3, *button4, *button5, *button6,
      *button7, *button8, *button9, *button10, *button11, *button12, *button13;
  GtkWidget *entry;
  GtkWidget *label;
  GtkWidget *image;
  GtkWidget *layout;

  total_tests = show_total_new_test(counties);
  total_infect = show_total_new_infect(counties);

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Covid-Status");
  gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  // pozitionam si setam dimensiunea ferestrei
  image = gtk_image_new_from_file("./covidtest2.jpg");
  layout = gtk_layout_new(NULL, NULL);
  gtk_layout_put(GTK_LAYOUT(layout), image, 0, 0);
  gtk_widget_show(layout);
  gtk_container_add(GTK_CONTAINER(window), layout);
  // aplicam imaginea de fundal

  label = gtk_label_new(
      "Protejeaza-te pe tine si pe cei din jur, folosind corect masca de "
      "protectie. Ce doriti sa aflati?\n\n");
  gtk_layout_put(GTK_LAYOUT(layout), label, 45, 10);

  // butoane cu funtii specifice,
  button1 = gtk_button_new_with_label("Situatia pe fiecare judet + Bucuresti");
  g_signal_connect(button1, "clicked", G_CALLBACK(print_total_situation), NULL);
  gtk_layout_put(GTK_LAYOUT(layout), button1, 200, 45);
  button2 = gtk_button_new_with_label("Numarul de teste de ieri");
  g_signal_connect(button2, "clicked", G_CALLBACK(print_tests), &total_tests);
  gtk_layout_put(GTK_LAYOUT(layout), button2, 240, 70);
  button3 = gtk_button_new_with_label("Numarul de infectari noi");
  g_signal_connect(button3, "clicked", G_CALLBACK(print_new_infect),
                   &total_infect);
  gtk_layout_put(GTK_LAYOUT(layout), button3, 240, 95);
  button4 = gtk_button_new_with_label("Rata de infectare Romania");
  g_signal_connect(button4, "clicked", G_CALLBACK(print_infection_rate),
                   &infection_rat);
  gtk_layout_put(GTK_LAYOUT(layout), button4, 235, 120);
  button5 =
      gtk_button_new_with_label("Sortare judete dupa rata infectarii pe judet");
  g_signal_connect_swapped(button5, "clicked",
                           G_CALLBACK(print_inf_rate_situation), &counties);
  gtk_layout_put(GTK_LAYOUT(layout), button5, 190, 145);
  button6 =
      gtk_button_new_with_label("Sortare judete dupa numarul de infectari noi");
  g_signal_connect_swapped(button6, "clicked",
                           G_CALLBACK(print_infections_situation), &counties);
  gtk_layout_put(GTK_LAYOUT(layout), button6, 180, 275);
  button7 = gtk_button_new_with_label("Judetul cu cele mai putine infectari");
  g_signal_connect_swapped(button7, "clicked", G_CALLBACK(print_least_infected),
                           NULL);
  gtk_layout_put(GTK_LAYOUT(layout), button7, 220, 300);
  button8 = gtk_button_new_with_label("Judetul cu cele mai multe infectari");
  g_signal_connect_swapped(button8, "clicked", G_CALLBACK(print_most_infected),
                           NULL);
  gtk_layout_put(GTK_LAYOUT(layout), button8, 220, 325);
  button9 =
      gtk_button_new_with_label("Judetul cu cea mai mica rata a infectarii");
  g_signal_connect_swapped(button9, "clicked", G_CALLBACK(print_min_inf_rate),
                           NULL);
  gtk_layout_put(GTK_LAYOUT(layout), button9, 200, 350);
  button10 =
      gtk_button_new_with_label("Judetul cu cea mai mare rata a infectarii");
  g_signal_connect_swapped(button10, "clicked", G_CALLBACK(print_max_inf_rate),
                           NULL);
  gtk_layout_put(GTK_LAYOUT(layout), button10, 200, 375);
  button11 = gtk_button_new_with_label("Simptome frecvente");
  g_signal_connect_swapped(button11, "clicked", G_CALLBACK(print_symptoms),
                           NULL);
  gtk_layout_put(GTK_LAYOUT(layout), button11, 260, 400);
  button12 = gtk_button_new_with_label("Cum sa ai grija de tine?");
  g_signal_connect_swapped(button12, "clicked", G_CALLBACK(print_tips), NULL);
  gtk_layout_put(GTK_LAYOUT(layout), button12, 250, 425);
  gtk_widget_show_all(window);
  // afiseaza pe ecran toate widgeturile
}

int main(int argc, char **argv) {
  read_input(counties);
  GtkApplication *app;
  int status;
  app = gtk_application_new("covid.status", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return 0;
}