 //app1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
    //Límites Maximos
#define MAX_LEN 100
#define MAX_ORDERS 1000
#define MAX_PIZZAS 1000
#define MAX_INGREDIENTS 100
#define MAX_CATEGORIES 20

//Estructuras de la app

typedef struct {    //Esta representa los datos agregados por día de venta, incluyendo fecha de la  venta, total de dinero recaudado por día y total de pizzas vendidas en esa fecha
    char date[20];    
    double total_sales;
    int total_pizzas;  
} SalesData;

typedef struct {    //Esta representa el total de ventas de cada tipo de pizza, incluyendo su nombre y la cantidad total vendida
    char pizza_name[50];
    int total_sold;
} PizzaData;

typedef struct {    //Esta representa el total de veces que se ha utilizado cada ingrediente, incluyendo el nombre del ingrediente y su frecuencia de uso en ventas
    char ingredient_name[50];
    int total_sold;
} IngredientData;

typedef struct {    //Esta representa las ventas acumuladas por categoría de pizza, incluyendo el nombre de la categoría y la cantidad total vendida en esa categoría
    char category_name[50];
    int total_sold;
} CategoryData;

typedef struct order { int dummy; } order;  //No se usa funcionalmente, es solo para cumplir con la firma de funciones requerida

//Datos globales 
  //Son datos utilizados para almacenar los totales acumulados y la información consolidada de ventas, pizzas, ingredientes y categorías, necesarios para el análisis y cálculo de métricas
int total_fechas = 0, total_pizzas = 0, orders_total = 0;
double total_revenue = 0;
int total_pizza_types = 0, total_ingredients = 0, total_categories = 0;

SalesData sales[MAX_ORDERS];    //Este arreglo almacena información de ventas por fecha
PizzaData pizzas[MAX_PIZZAS];   //Etse arreglo almacena datos acumulados por tipo de pizza
IngredientData ingredients[MAX_INGREDIENTS];  //Este rreglo almacena la frecuencia de uso de cada ingrediente
CategoryData categories[MAX_CATEGORIES];  //Este arreglo almacena los datos acumulados por categoría de pizza

//Funciones de métricas

char* metrica_apo(int *size, struct order *orders) {    //Esta calcula el promedio de pizzas vendidas por orden, dividiendo el total de pizzas por la cantidad de órdenes
    static char buffer[128];
    snprintf(buffer, sizeof(buffer), "Promedio de pizzas por orden: %.2f", (double)total_pizzas / orders_total);
    return buffer;
}

char* metrica_apd(int *size, struct order *orders) {    //Esta calcula el promedio de pizzas vendidas por día, dividiendo el total de pizzas por la cantidad de fechas registradas
    static char buffer[128];
    snprintf(buffer, sizeof(buffer), "Promedio de pizzas por día: %.2f", (double)total_pizzas / total_fechas);
    return buffer;
}

char* metrica_pms(int *size, struct order *orders) {    //Esta identifica la pizza más vendida del total registrado, recorriendo el arreglo y comparando las cantidades
    static char buffer[128];
    PizzaData best = pizzas[0];
    for (int i = 1; i < total_pizza_types; i++) {
        if (pizzas[i].total_sold > best.total_sold) best = pizzas[i];
    }
    snprintf(buffer, sizeof(buffer), "Pizza más vendida: %s - %d unidades", best.pizza_name, best.total_sold);
    return buffer;
}

char* metrica_pls(int *size, struct order *orders) {    //Esta identifica la pizza menos vendida del total registrado, recorriendo el arreglo y comparando las cantidades
    static char buffer[128];
    PizzaData worst = pizzas[0];
    for (int i = 1; i < total_pizza_types; i++) {
        if (pizzas[i].total_sold < worst.total_sold) worst = pizzas[i];
    }
    snprintf(buffer, sizeof(buffer), "Pizza menos vendida: %s - %d unidades", worst.pizza_name, worst.total_sold);
    return buffer;
}


char* metrica_dms(int *size, struct order *orders) {    // Identificar dia con mas dinero se gano dentro de sales
    static char buffer[128];
    SalesData best = sales[0];
    for (int i = 1; i < total_fechas; i++) {
        if (sales[i].total_sales > best.total_sales) best = sales[i];
    }
    snprintf(buffer, sizeof(buffer), "Fecha con más ventas en dinero: %s - %.2f", best.date, best.total_sales);
    return buffer;
}

char* metrica_dls(int *size, struct order *orders) {    // Identificar el dia con menos dinero se gano dentro de sales
    static char buffer[128];
    SalesData worst = sales[0];
    for (int i = 1; i < total_fechas; i++) {
        if (sales[i].total_sales < worst.total_sales) worst = sales[i];
    }
    snprintf(buffer, sizeof(buffer), "Fecha con menos ventas en dinero: %s - %.2f", worst.date, worst.total_sales);
    return buffer;
}

char* metrica_dmsp(int *size, struct order *orders) {   // Identificar el dia que mas pizzas se vendieron 
    static char buffer[128];
    SalesData best = sales[0];
    for (int i = 1; i < total_fechas; i++) {
        if (sales[i].total_pizzas > best.total_pizzas) best = sales[i];
    }
    snprintf(buffer, sizeof(buffer), "Fecha con más ventas en cantidad de pizzas: %s - %d", best.date, best.total_pizzas);
    return buffer;
}

char* metrica_dlsp(int *size, struct order *orders) {   // Identificar el dia que menos pizzas se vendieron
    static char buffer[128];
    SalesData worst = sales[0];
    for (int i = 1; i < total_fechas; i++) {
        if (sales[i].total_pizzas < worst.total_pizzas) worst = sales[i];
    }
    snprintf(buffer, sizeof(buffer), "Fecha con menos ventas en cantidad de pizzas: %s - %d", worst.date, worst.total_pizzas);
    return buffer;
}

char* metrica_ims(int *size, struct order *orders) {    // Identificar cual fue el ingrediente mas vendido
    static char buffer[128];
    IngredientData best = ingredients[0];
    for (int i = 1; i < total_ingredients; i++) {
        if (ingredients[i].total_sold > best.total_sold) best = ingredients[i];
    }
    snprintf(buffer, sizeof(buffer), "Ingrediente más vendido: %s - %d unidades", best.ingredient_name, best.total_sold);
    return buffer;
}

char* metrica_hp(int *size, struct order *orders) {     // Resumen para mostrar la cantidad de pizzas vendidas por categoria
    static char buffer[1024];
    buffer[0] = '\0';
    strcat(buffer, "Cantidad de pizzas por categoría:\n");
    for (int i = 0; i < total_categories; i++) {
        char linea[100];
        snprintf(linea, sizeof(linea), "- %s: %d unidades\n", categories[i].category_name, categories[i].total_sold);
        strcat(buffer, linea);
    }
    return buffer;
}
// --- Arreglo de funciones y nombres ---
typedef char* (*MetricaFunc)(int *, struct order *);

const char *nombres_metricas[] = {
    "apo", "apd", "pms", "pls",
    "dms", "dls", "dmsp", "dlsp",
    "ims", "hp"
};

MetricaFunc funciones_metricas[] = {
    metrica_apo, metrica_apd, metrica_pms, metrica_pls,
    metrica_dms, metrica_dls, metrica_dmsp, metrica_dlsp,
    metrica_ims, metrica_hp
};

int num_metricas = sizeof(nombres_metricas) / sizeof(nombres_metricas[0]);

// --- FunciÃ³n para cargar los datos del archivo CSV ---
void cargar_datos(const char *archivo) {
    FILE *fp = fopen(archivo, "r");
    if (!fp) {
        perror("Error al abrir el archivo");
        exit(1);
    }

    char linea[1024];
    int order_counts[MAX_ORDERS] = {0};
    char orders_list[MAX_ORDERS][20];

    fgets(linea, sizeof(linea), fp); // saltar encabezado
    while (fgets(linea, sizeof(linea), fp)) {
        char *temp;
        int order_id, quantity;
        double total_price;
        char order_date[20], pizza_name[50], pizza_category[50], pizza_ingredients[200];
        char pizza_size[10];

        temp = strtok(linea, ","); temp = strtok(NULL, ","); order_id = atoi(temp);
        temp = strtok(NULL, ","); temp = strtok(NULL, ","); quantity = atoi(temp);
        temp = strtok(NULL, ","); strcpy(order_date, temp);
        temp = strtok(NULL, ","); // order_time (ignorado)
        temp = strtok(NULL, ","); // unit_price (ignorado)
        temp = strtok(NULL, ","); if (temp) total_price = atof(temp); // total_price correcto        
        total_price = atof(temp);
        temp = strtok(NULL, ","); if (temp) strcpy(pizza_size, temp);           // pizza_size correcto
        temp = strtok(NULL, ","); if (temp) strcpy(pizza_category, temp);      // pizza_category correcto
        temp = strtok(NULL, "\""); if (temp) strcpy(pizza_ingredients, temp);  // pizza_ingredients correcto        
        temp = strtok(NULL, ",");
        if (temp) {
            if (temp[0] == '"') memmove(temp, temp + 1, strlen(temp));
            temp[strlen(temp) - 1] = '\0';
            strcpy(pizza_name, temp);
        } else strcpy(pizza_name, "Unknown Pizza");

        order_counts[order_id] += quantity;
        total_pizzas += quantity;
        total_revenue += total_price;

        char order_id_str[20]; sprintf(order_id_str, "%d", order_id);
        int found = 0;
        for (int i = 0; i < orders_total; i++) {
            if (strcmp(orders_list[i], order_id_str) == 0) { found = 1; break; }
        }
        if (!found) { strcpy(orders_list[orders_total++], order_id_str); }
    
        found = 0;
        for (int i = 0; i < total_fechas; i++) {
            if (strcmp(sales[i].date, order_date) == 0) {
                sales[i].total_sales += total_price;
                sales[i].total_pizzas += quantity;
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(sales[total_fechas].date, order_date);
            sales[total_fechas].total_sales = total_price;
            sales[total_fechas++].total_pizzas = quantity;
        }

        found = 0;
        for (int i = 0; i < total_pizza_types; i++) {
            if (strcmp(pizzas[i].pizza_name, pizza_name) == 0) {
                pizzas[i].total_sold += quantity; found = 1; break;
            }
        }
        if (!found) {
            strcpy(pizzas[total_pizza_types].pizza_name, pizza_name);
            pizzas[total_pizza_types++].total_sold = quantity;
        }

        found = 0;
        for (int i = 0; i < total_categories; i++) {
            if (strcmp(categories[i].category_name, pizza_category) == 0) {
                categories[i].total_sold += quantity; found = 1; break;
            }
        }
        if (!found) {
            strcpy(categories[total_categories].category_name, pizza_category);
            categories[total_categories++].total_sold = quantity;
        }

        if (pizza_ingredients[0] == '"') {
            memmove(pizza_ingredients, pizza_ingredients + 1, strlen(pizza_ingredients));
            pizza_ingredients[strlen(pizza_ingredients) - 1] = '\0';
        }
        for (int i = 0; pizza_ingredients[i]; i++) {
            if (pizza_ingredients[i] == ',') pizza_ingredients[i] = ';';
        }

        char *ingredient = strtok(pizza_ingredients, ";");
        while (ingredient) {
            while (*ingredient == ' ') ingredient++;
            char *end = ingredient + strlen(ingredient) - 1;
            while (end > ingredient && (*end == ' ' || *end == '"')) *end-- = '\0';

            found = 0;
            for (int i = 0; i < total_ingredients; i++) {
                if (strcmp(ingredients[i].ingredient_name, ingredient) == 0) {
                    ingredients[i].total_sold += quantity; found = 1; break;
                }
            }
            if (!found && total_ingredients < MAX_INGREDIENTS) {
                strcpy(ingredients[total_ingredients].ingredient_name, ingredient);
                ingredients[total_ingredients++].total_sold = quantity;
            }
            ingredient = strtok(NULL, ";");
        }
    }
    fclose(fp);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s <archivo_csv> <metrica1> [<metrica2> ...]\n", argv[0]);
        return 1;
    }

    cargar_datos(argv[1]);
    int dummy = 0;
    struct order *ordenes = NULL;

    for (int i = 2; i < argc; i++) {
        for (int j = 0; j < num_metricas; j++) {
            if (strcmp(argv[i], nombres_metricas[j]) == 0) {
                printf("\n--- %s ---\n%s\n", argv[i], funciones_metricas[j](&dummy, ordenes));
                break;
            }
        }
    }

    return 0;
}
