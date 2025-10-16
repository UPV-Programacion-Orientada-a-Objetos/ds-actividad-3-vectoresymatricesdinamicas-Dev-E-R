#include <iostream>


// estructura de la matriz
const int num_productos = 5;
const int num_materias_primas = 4;


// estructuras dinamicas 
float** matrizPlanificacion = nullptr;
float* vectorCostos = nullptr;
float* inventarioMateriaPrima;

// requerimientos
// MP = Materia prima
int requerimientosMP[num_productos][num_materias_primas];

// variable para mantener control del tama;o de la matriz
int semanasPlanificadas = 0;

// inicializacion de funciones principales
void startSystem(int);
void menu();
void productionPlan();
void addNewWeek();
void modifyProduction();
void calcCOGSnFinalInventory();
void cleanMemory();


int main() {
    // Inicializacion del sistema de 4 semanas (definido)
    startSystem(4);

    menu();

    cleanMemory();
    return 0;
}

void addNewWeek(){
    int nuevasSemanas = semanasPlanificadas + 1;
    std::cout << "Agregando Semana " << nuevasSemanas << "..." << std::endl;

    float** nuevaMatriz = new float*[num_productos];
    for (int i = 0; i < num_productos; ++i) {
        nuevaMatriz[i] = new float[nuevasSemanas];
    }

    for (int i = 0; i < num_productos; ++i) {
        for (int j = 0; j < semanasPlanificadas; ++j) {
            nuevaMatriz[i][j] = matrizPlanificacion[i][j];
        }
        nuevaMatriz[i][semanasPlanificadas] = 0;
    }

    for (int i = 0; i < num_productos; ++i) {
        delete[] matrizPlanificacion[i];
    }
    delete[] matrizPlanificacion;

    matrizPlanificacion = nuevaMatriz;
    semanasPlanificadas = nuevasSemanas; 

    std::cout << "Matriz redimensionada a " << num_productos << "x"
              << semanasPlanificadas << ". Memoria gestionada con exito!" << std::endl;
}

void startSystem(int semanasIniciales){
    std::cout << "Inicializando sistema...." << std::endl;
    // Inicia por default con 4 semanas iniciales
    semanasPlanificadas = semanasIniciales;
    // matriz
    matrizPlanificacion = new float*[num_productos];
    for (int i = 0; i < num_productos; i++)
    {
        matrizPlanificacion[i] = new float[semanasPlanificadas];
        for (int j = 0; j < semanasPlanificadas; j++)
        {
            matrizPlanificacion[i][j] = (i + 1) * 100 + (j + 1) * 10;
        }
        
    }
    // vector
    vectorCostos = new float[num_productos];
    inventarioMateriaPrima = new float[num_materias_primas];

    // generacion de datos (ficticios)
    inventarioMateriaPrima[0] = 1000; inventarioMateriaPrima[1] = 1500;
    inventarioMateriaPrima[2] = 2000; inventarioMateriaPrima[3] = 800;

    int reqs[num_productos][num_materias_primas] = {
        {2, 1, 0, 0}, {1, 0, 3, 0}, {0, 2, 1, 1}, {3, 0, 0, 2}, {1, 1, 1, 1}
    };
    for (int i = 0; i < num_productos; ++i) {
        for (int j = 0; j < num_materias_primas; ++j) {
            requerimientosMP[i][j] = reqs[i][j];
        }
    }

    std::cout << "Matriz redimensionada a " << num_productos << " x " << semanasPlanificadas << ". -- Memoria gnerada -- " << std::endl;
}

void calcCOGSnFinalInventory(){
    int semana;
    std::cout << "Ingresa semana para el calculo de costos (0 - " << semanasPlanificadas - 1 << "): ";
    std::cin >> semana;

    if (semana < 0 || semana >= semanasPlanificadas) {
        std::cout << "Error: Semana invalida." << std::endl;
        return;
    }

    float cogsTotal = 0;
    float produccionTotalSemana = 0;
    float consumoMP[num_materias_primas] = {0};

    // Calcular COGS y Consumo de MP
    for (int i = 0; i < num_productos; i++)
    {
        float unidadesProducidas = matrizPlanificacion[i][semana];
        produccionTotalSemana += unidadesProducidas;
        cogsTotal += unidadesProducidas * vectorCostos[i];
        for (int j = 0; j < num_materias_primas; ++j) {
            consumoMP[j] += unidadesProducidas * requerimientosMP[i][j];
        }
    }
    
    // inventario final
    float costosAdquisicionMP[num_materias_primas] = {2.0, 1.5, 3.0, 5.0};
    float valorInventarioFinal = 0;
    for (int i = 0; i < num_materias_primas; ++i) {
        float mpRestante = inventarioMateriaPrima[i] - consumoMP[i];
        valorInventarioFinal += (mpRestante > 0 ? mpRestante : 0) * costosAdquisicionMP[i];
    }

    // Mostrar Reporte
    std::cout << "\n--- Reporte de Costos (Semana " << semana + 1 << ") ---" << std::endl;
    std::cout << "Produccion Total: " << produccionTotalSemana << " unidades." << std::endl;
    std::cout << "Costo Total de Produccion (COGS): $" << cogsTotal << std::endl;
    std::cout << "Consumo de Materia Prima: ";
    for (int i = 0; i < num_materias_primas; ++i) {
        std::cout << "MP-0" << i << " (" << consumoMP[i] << " uni) ";
    }
    std::cout << "\nValor del Inventario Final (M.P. restante): $" << valorInventarioFinal << std::endl;
}

void modifyProduction(){
    int prod, sem;
    float cant;
    std::cout << "Ingrese Producto (0-" << num_productos - 1 << "): "; std::cin >> prod;
    std::cout << "Ingrese Semana (0-" << semanasPlanificadas - 1 << "): "; std::cin >> sem;
    std::cout << "Cantidad a producir: "; 
    std::cin >> cant;

    if (prod >= 0 && prod < num_productos && sem >= 0 && sem < semanasPlanificadas && cant >= 0) {
        matrizPlanificacion[prod][sem] = cant;
        std::cout << "Produccion actualizada con exito." << std::endl;
    } else {
        std::cout << "Datos invalidos. No se pudo actualizar." << std::endl;
    }
}

void productionPlan(){
    std::cout << "\n--- Plan de Produccion Actual ---" << std::endl;
    std::cout << "Producto";
    for(int j=0; j<semanasPlanificadas; ++j) {
        std::cout << " Sem " << j + 1 << " - ";
    }
    std::cout << std::endl << std::string(10 + 10 * semanasPlanificadas, '-') << std::endl;

    for (int i = 0; i < num_productos; ++i) {
        std::cout << "Prod \t " + std::to_string(i);
        for (int j = 0; j < semanasPlanificadas; ++j) {
            std::cout << matrizPlanificacion[i][j];
            std::cout << " - ";
        }
        std::cout << std::endl;
    }
}
void menu(){
    int opcion;

    do
    {
        std::cout << "--- Menu Principal ---" << std::endl;
        std::cout << "1. Ver Plan de Produccion" << std::endl;
        std::cout << "2. Agregar Nueva Semana (Redimensionar Matriz)" << std::endl;
        std::cout << "3. Modificar Produccion" << std::endl;
        std::cout << "4. Calcular COGS y Final Iventory" << std::endl;
        std::cout << "5. Salir" << std::endl;

        std::cin >> opcion;

        switch (opcion)
        {
        case 1:
            productionPlan();
            break;
        case 2:
            addNewWeek();
            break;
        case 3:
            modifyProduction();
            break;
        case 4:
            calcCOGSnFinalInventory();
            break;
        case 5:
            std::cout << "Saliendo...." << std::endl;
        default:
            break;
        }
    } while (opcion != 5);
    
}

void cleanMemory(){
    std::cout << "\nLiberando memoria de Matriz y Vectores Dinamicos..." << std::endl;

    if (matrizPlanificacion != nullptr) {
        // Liberacion en orden inverso
        for (int i = 0; i < num_productos; ++i) {
            delete[] matrizPlanificacion[i];
        }
        delete[] matrizPlanificacion;
    }

    // Liberamos los vectores
    delete[] vectorCostos;
    delete[] inventarioMateriaPrima;

    std::cout << "Sistema cerrado." << std::endl;
}