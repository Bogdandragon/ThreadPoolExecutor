/* ANGHELOIU George-Bogdan - grupa 314CB */
#include <stdio.h>
#include "tlg.h"

typedef struct {
    char ID;
    unsigned long timeElapsed;
} Thread;

typedef struct {
    short ID;
    Thread thread;
    char priority;
    unsigned long execTime;
} Task;

typedef struct {
    TLG start;
    TLG end;
} TSC;


TLG* InitThreads(char N) {
    char i = N - 1;
    TLG* aux = (TLG*)malloc(sizeof(TLG));
    *aux = NULL;
    Thread* info = NULL;
    for(; i >= 0; i--){
        info = (Thread*)malloc(sizeof(Thread));
        info->ID = i;
        if(!Ins_DecLG(aux, info)) {
            return NULL;
        }
    }
    return aux;
}

short firstIDAvailable(char IDs[], short* last) {
    short i = 1;
    for(; i <= *last; i++)
        if(IDs[i])
            return i;
    (*last)++;
    return *last;
}

TLG Pop(TLG* stiva) {
    TLG elem;
    if(*stiva == NULL)
        return NULL;
    else if((*stiva)->prec == *stiva) {
        elem = *stiva;
        *stiva = NULL;
    } else {
        elem = (*stiva)->prec;
        elem->prec->urm = *stiva;
        (*stiva)->prec = elem->prec;
    }
    elem->prec = NULL;
    elem->urm = NULL;
    return elem;
}

void Push(TLG* stiva, TLG elem) {
    if(*stiva == NULL) {
        *stiva = elem;
        elem->prec = elem;
        elem->urm = elem;
    } else {
        (*stiva)->prec->urm = elem;
        elem->prec = (*stiva)->prec;
        elem->urm = *stiva;
        (*stiva)->prec = elem;
    }
}

int threadCmp(void* el1, void* el2) {
    Thread* thread1 = (Thread*)el1;
    Thread* thread2 = (Thread*)el2;
    if(thread1->ID < thread2->ID)
        return -1;
    else if(thread1->ID == thread2->ID)
        return 0;
    else return 1;
}

void PushCond(TLG* stiva, int (*fCmp)(void*, void*), TLG item) {
    if(*stiva == NULL) {
        *stiva = item;
        item->prec = item;
        item->urm = item;
        return;
    }
    TLG* aux = malloc(sizeof(TLG));
    *aux = NULL;
    TLG elem;
    TLG moved = item;
    if(fCmp((*stiva)->info, moved->info) < 0) {
        moved->urm = *stiva;
        moved->prec = (*stiva)->prec;
        (*stiva)->prec->urm = moved;
        (*stiva)->prec = moved;
        *stiva = moved;
        free(aux);
        return;
    }
    while(*stiva) {
        elem = Pop(stiva);
        if(moved != NULL && fCmp(elem->info, moved->info) <= 0){
            Push(aux, moved);
            moved = NULL;
        }
        Push(aux, elem);
    }
    if(moved != NULL)
    {
        Push(aux, moved);
        item = NULL;
    }
    while(*aux) {
        elem = Pop(aux);
        Push(stiva, elem);
    }
    free(aux);
}

int taskCmp(void* el1, void* el2) {
    Task* task1 = (Task*)el1;
    Task* task2 = (Task*)el2;
    if(task1->priority < task2->priority) {
        return -1;
    } else if(task1->priority == task2->priority) {
        if(task1->execTime > task2->execTime)
            return -1;
        else if(task1->execTime == task2->execTime) {
            if(task1->ID > task2->ID)
                return -1;
            else if(task1->ID == task2->ID)
                return 0;
            else
                return 1;
        } else 
            return 1;
    } else
        return 1;
}

TLG ExtrQ(TLG* coada) {
    TLG elem;
    if(*coada == NULL)
        return NULL;
    if((*coada)->prec == *coada) {
        elem = *coada;
        *coada = NULL;
    } else {
        elem = *coada;
        (*coada)->prec->urm = elem->urm;
        elem->urm->prec = (*coada)->prec;
        *coada = elem->urm;
    }
    elem->urm = NULL;
    elem->prec = NULL;
    return elem;
}

void InsQ(TLG* coada, TLG elem) {
    if(*coada == NULL) {
        *coada = elem;
        elem->urm = elem;
        elem->prec = elem;
    } else {
        elem->prec = (*coada)->prec;
        elem->urm = *coada;
        (*coada)->prec->urm = elem;
        (*coada)->prec = elem;
    }
}

int InsQCond(TLG* coada, int (*fCmp)(void*, void*), Task* task) {
    if(*coada == NULL) {
        int rez = Ins_DecLG(coada, task);
        return rez;
    }
    TLG* aux = malloc(sizeof(TLG));
    *aux = NULL;
    TLG elem;
    TLG new = Aloc_CelulaG(task);
    if(fCmp((*coada)->info, task) < 0) {
        new->urm = *coada;
        new->prec = (*coada)->prec;
        (*coada)->prec->urm = new;
        (*coada)->prec = new;
        *coada = new;
        free(aux);
        return 1;
    }
    while(*coada) {
        elem = ExtrQ(coada);
        if(new != NULL && fCmp(elem->info, task) <= 0){
            InsQ(aux, new);
            new = NULL;
        }
        InsQ(aux, elem);
    }
    if(new != NULL)
    {
        InsQ(aux, new);
        new = NULL;
    }
    while(*aux) {
        elem = ExtrQ(aux);
        InsQ(coada, elem);
    }
    free(aux);
    return 1;
}

int InsQCondLG(TLG* coada, int (*fCmp)(void*, void*), TLG item) {
    if(*coada == NULL) {
        InsQ(coada, item);
        return 1;
    }
    TLG* aux = malloc(sizeof(TLG));
    *aux = NULL;
    TLG elem;
    TLG moved = item;
    if(fCmp((*coada)->info, moved->info) < 0) {
        moved->urm = *coada;
        moved->prec = (*coada)->prec;
        (*coada)->prec->urm = moved;
        (*coada)->prec = moved;
        *coada = moved;
        free(aux);
        return 1;
    }
    while(*coada) {
        elem = ExtrQ(coada);
        if(moved != NULL && fCmp(elem->info, moved->info) <= 0){
            InsQ(aux, moved);
            moved = NULL;
        }
        InsQ(aux, elem);
    }
    if(moved != NULL)
    {
        InsQ(aux, moved);
        item = NULL;
    }
    while(*aux) {
        elem = ExtrQ(aux);
        InsQ(coada, elem);
    }
    free(aux);
    return 1;
}

void printThread(void* info) {
    Thread* thread = (Thread*)info;
    printf("%d\n", thread->ID);
}

void printStiva(TLG* stiva, void (*afiEL)(void*)) {
    if(!stiva) {
        return;
    }
    TLG* aux = malloc(sizeof(TLG));
    *aux = NULL;
    TLG elem;
	while(*stiva) {
        elem = Pop(stiva);
        Push(aux, elem);
    }
    while(*aux) {
        elem = Pop(aux);
        afiEL(elem->info);
        Push(stiva, elem);
    }
    free(aux);
}

void printWaiting(TLG* coada, FILE* out) {
    TLG elem;
    Task* task;
    TLG* aux = malloc(sizeof(TLG));
    *aux = NULL;
    fprintf(out, "====== Waiting queue =======\n[");
    while(*coada) {
        elem = ExtrQ(coada);
        task = (Task*)elem->info;
        fprintf(out, "(%d: priority = %d, remaining_time = %ld)", task->ID,
            task->priority, task->execTime);
        if(*coada)
            fprintf(out, ",\n");
        InsQ(aux, elem);
    }
    while(*aux) {
        elem = ExtrQ(aux);
        InsQ(coada, elem);
    }
    fprintf(out, "]\n");
    free(aux);
}

void printRunning(TLG* coada, FILE* out) {
    TLG elem;
    Task* task;
    TLG* aux = malloc(sizeof(TLG));
    *aux = NULL;
    fprintf(out, "====== Running in parallel =======\n[");
    while(*coada) {
        elem = ExtrQ(coada);
        task = (Task*)elem->info;
        fprintf(out, "(%d: priority = %d, remaining_time = %ld, running_thread = %hhd)",
            task->ID, task->priority, task->execTime - task->thread.timeElapsed, task->thread.ID);
        if(*coada)
            fprintf(out, ",\n");
        InsQ(aux, elem);
    }
    while(*aux) {
        elem = ExtrQ(aux);
        InsQ(coada, elem);
    }
    fprintf(out, "]\n");
    free(aux);
}

void printFinished(TLG* coada, FILE* out) {
    TLG elem;
    Task* task;
    TLG* aux = malloc(sizeof(TLG));
    *aux = NULL;
    fprintf(out, "====== Finished queue =======\n[");
    while(*coada) {
        elem = ExtrQ(coada);
        task = (Task*)elem->info;
        fprintf(out, "(%d: priority = %d, executed_time = %ld)", task->ID,
            task->priority, task->execTime);
        if(*coada)
            fprintf(out, ",\n");
        InsQ(aux, elem);
    }
    while(*aux) {
        elem = ExtrQ(aux);
        InsQ(coada, elem);
    }
    free(aux);
    fprintf(out, "]\n");
}

Task* findTaskById(TLG* coada, short id) {
    TLG* aux = malloc(sizeof(TLG));
    *aux = NULL;
    TLG elem, gasit = NULL;
    while(*coada) {
        elem = ExtrQ(coada);
        if(gasit == NULL && ((Task*)(elem->info))->ID == id)
            gasit = elem;
        InsQ(aux, elem);
    }
    while(*aux) {
        elem = ExtrQ(aux);
        InsQ(coada, elem);
    }
    free(aux);
    return gasit == NULL ? NULL : gasit->info;
}

Task* findTaskByThread(TLG* coada, char thread) {
    TLG* aux = malloc(sizeof(TLG));
    *aux = NULL;
    TLG elem, gasit = NULL;
    while(*coada) {
        elem = ExtrQ(coada);
        if(gasit == NULL && ((Task*)(elem->info))->thread.ID == thread)
            gasit = elem;
        InsQ(aux, elem);
    }
    while(*aux) {
        elem = ExtrQ(aux);
        InsQ(coada, elem);
    }
    free(aux);
    return gasit == NULL ? NULL : gasit->info;
}

int idleThread(TLG* stiva, char id) {
    TLG* aux = malloc(sizeof(TLG));
    *aux = NULL;
    TLG elem, gasit = NULL;
    while(*stiva) {
        elem = Pop(stiva);
        if(gasit == NULL && ((Thread*)(elem->info))->ID == id)
            gasit = elem;
        Push(aux, elem);
    }
    while(*aux) {
        elem = Pop(aux);
        Push(stiva, elem);
    }
    free(aux);
    return gasit != NULL;
}

unsigned long minim(TLG* coada, unsigned long maxim) {
    TLG* aux = malloc(sizeof(TLG));
    unsigned long minimumTime = 1001;
    *aux = NULL;
    TLG elem;
    while(*coada) {
        elem = ExtrQ(coada);
        if(((Task*)(elem->info))->execTime - ((Task*)(elem->info))->thread.timeElapsed
            < minimumTime)
            minimumTime = ((Task*)(elem->info))->execTime -
                ((Task*)(elem->info))->thread.timeElapsed;
        InsQ(aux, elem);
    }
    while(*aux) {
        elem = ExtrQ(aux);
        InsQ(coada, elem);
    }
    free(aux);
    return maxim < minimumTime ? maxim : minimumTime;
}

void moveTime(TLG* threads, TLG* running, TLG* finished, char taskIds[], unsigned long time) {
    TLG* aux = malloc(sizeof(TLG));
    *aux = NULL;
    TLG elem;
    Task* task;
    Thread* thread;
    while(*running) {
        elem = ExtrQ(running);
        task = (Task*)(elem->info);
        if(task->execTime - task->thread.timeElapsed == time) {
            InsQ(finished, elem);
            taskIds[task->ID] = 1;
            thread = malloc(sizeof(Thread));
            thread->ID = task->thread.ID;
            thread->timeElapsed = 0;
            elem = Aloc_CelulaG(thread);
            PushCond(threads, threadCmp, elem);
        } else {
            task->thread.timeElapsed += time;
            InsQ(aux, elem);
        }
    }
    while(*aux) {
        elem = ExtrQ(aux);
        InsQCondLG(running, taskCmp, elem);
    }
    free(aux);
}

void run(TLG* threads, TLG* waiting, TLG* running, TLG* finished,
    char taskIds[], unsigned long totalTime, unsigned long Q) {
    TLG thread, task;
    TLG* auxThreads = malloc(sizeof(TLG));
    *auxThreads = NULL;
    TLG* auxWaiting = malloc(sizeof(TLG));
    *auxWaiting = NULL;
    TLG* auxRunning = malloc(sizeof(TLG));
    *auxRunning = NULL;
    TLG* auxFinished = malloc(sizeof(TLG));
    *auxFinished = NULL;
    unsigned long timePassed = 0;
    unsigned long minimumTime = 1001;
    while(totalTime >= timePassed) {
        // ocupam thread-urile cu ce se afla in waiting
        while(*threads && *waiting) {
            thread = Pop(threads);
            task = ExtrQ(waiting);
            ((Task*)(task->info))->thread = *((Thread*)(thread->info));
            ((Task*)(task->info))->thread.timeElapsed = 0;
            free(thread->info);
            free(thread);
            InsQCondLG(running, taskCmp, task);
        }
        while(*threads) {
            thread = Pop(threads);
            Push(auxThreads, thread);
        }
        while(*auxThreads) {
            thread = Pop(auxThreads);
            Push(threads, thread);
        }
        while(*waiting) {
            task = ExtrQ(waiting);
            Push(auxWaiting, task);
        }
        while(*auxWaiting) {
            task = ExtrQ(auxWaiting);
            Push(waiting, task);
        }
        if(totalTime == timePassed)
            break;

        // update timp la taskuri
        if(Q < totalTime - timePassed)
            minimumTime = minim(running, Q);
        else
            minimumTime = minim(running, totalTime - timePassed);
        moveTime(threads, running, finished, taskIds, minimumTime);
        timePassed += minimumTime;
    }
    free(auxFinished);
    free(auxRunning);
    free(auxWaiting);
    free(auxThreads);
}

void DistrugeC(TLG* coada) {
    TLG elem;
    while(*coada) {
        elem = ExtrQ(coada);
        free(elem->info);
        free(elem);
    }
    free(coada);
}
void DistrugeS(TLG* stiva) {
    TLG elem;
    while(*stiva) {
        elem = Pop(stiva);
        free(elem->info);
        free(elem);
    }
    free(stiva);
}

int main(int argc, char* argv[]) {
    FILE* in = fopen(argv[1], "r");
    fclose(in);
    in = fopen(argv[1], "r");
    FILE* out = fopen(argv[2], "w");
    short Q;
    char C;
    fscanf(in, "%hd", &Q);
    fscanf(in, "%hhd", &C);
    char N = 2 * C;
    TLG *waiting = malloc(sizeof(TLG)), *running = malloc(sizeof(TLG)),
        *finished = malloc(sizeof(TLG));
    *waiting = *running = *finished = NULL;
    TLG* threads  = InitThreads(N);
    if(!threads)
        return 0;
    char* command = malloc(1000);
    //available task IDs
    char taskIDs[32768];
    memset(taskIDs, 1, 32768);
    short lastID = 0;
    int nr, i;
    short id;
    char priority, threadId;
    unsigned long execTime;
    Task* task;
    unsigned long totalTime = 0;
    while(fscanf(in, " %s", command) > 0) {
        if(strcmp(command, "add_tasks") == 0) {
            fscanf(in, " %d", &nr);
            fscanf(in, " %lu", &execTime);
            fscanf(in, " %hhd", &priority);
            for(i = 1; i <= nr; i++){
                task = malloc(sizeof(Task));
                if(!task){
                    break;
                }
                task->ID = firstIDAvailable(taskIDs, &lastID);
                taskIDs[task->ID] = 0;
                task->execTime = execTime;
                task->priority = priority;
                if(!InsQCond(waiting, taskCmp, task)) {
                    break;
                } else {
                    fprintf(out,"Task created successfully : ID %d.\n", task->ID);
                }
            }
        } else if (strcmp(command, "get_task") == 0) {
            fscanf(in, " %hd", &id);
            if(taskIDs[id]) {
                task = findTaskById(finished, id);
                if(task)
                    fprintf(out, "Task %d is finished (executed_time = %lu).\n", id, task->execTime);
                else
                    fprintf(out, "Task %d not found.\n", id);
            } else {
                task = findTaskById(running, id);
                if(task)
                    fprintf(out, "Task %d is running (remaining_time = %lu).\n", id,
                        task->execTime - task->thread.timeElapsed);
                else {
                    task = findTaskById(waiting, id);
                    if(task)
                        fprintf(out, "Task %d is waiting (remaining_time = %lu).\n", id, task->execTime);
                    else
                        fprintf(out, "Task %d not found.\n", id);
                }
            }
        } else if (strcmp(command, "get_thread") == 0) {
            fscanf(in, " %hhd", &threadId);
            if(idleThread(threads,threadId))
                fprintf(out, "Thread %hhd is idle.\n", threadId);
            else {
                task = findTaskByThread(running, threadId);
                if(task)
                    fprintf(out, "Thread %hhd is running task %hd (remaining_time = %lu).\n",
                        threadId, task->ID, task->execTime);
            }
        } else if (strcmp(command, "print") == 0){
            fscanf(in, " %s", command);
            if(strcmp(command, "waiting") == 0)
                printWaiting(waiting, out);
            if(strcmp(command, "running") == 0)
                printRunning(running, out);
            if(strcmp(command, "finished") == 0)
                printFinished(finished, out);
        } else if (strcmp(command, "run") == 0) {
            fscanf(in, " %lu", &execTime);
            fprintf(out, "Running tasks for %lu ms...\n", execTime);
            run(threads, waiting, running, finished, taskIDs, execTime, Q);
            totalTime += execTime;
        } else {
            fprintf(out, "Total time: %lu", totalTime);
            break;
        }
    }
    fclose(in);
    fclose(out);
    DistrugeS(threads);
    DistrugeC(waiting);
    DistrugeC(running);
    DistrugeC(finished);
    free(command);
    return 0;
}