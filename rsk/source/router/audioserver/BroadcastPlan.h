#ifndef BROADCAST_PLAN_H
#define BROADCAST_PLAN_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

typedef struct {
    int id;
    char* name;
    char** audioFiles;
    size_t audioFileCount;
} BroadcastPlan;

typedef struct {
    BroadcastPlan* plans;
    size_t planCount;
    char* configPath;
} BroadcastPlanManager;

BroadcastPlan* create_broadcast_plan(int id, const char* name, char** audioFiles, size_t audioFileCount);
void destroy_broadcast_plan(BroadcastPlan* plan);
json_t* broadcast_plan_to_json(const BroadcastPlan* plan);
BroadcastPlan* broadcast_plan_from_json(const json_t* json);

BroadcastPlanManager* create_broadcast_plan_manager(const char* configPath);
void destroy_broadcast_plan_manager(BroadcastPlanManager* manager);
bool load_broadcast_plan_manager(BroadcastPlanManager* manager);
bool save_broadcast_plan_manager(const BroadcastPlanManager* manager);
void add_plan_to_manager(BroadcastPlanManager* manager, BroadcastPlan* plan);
void remove_plan_from_manager(BroadcastPlanManager* manager, int id);
BroadcastPlan* find_plan_in_manager(BroadcastPlanManager* manager, int id);

#endif // BROADCAST_PLAN_H