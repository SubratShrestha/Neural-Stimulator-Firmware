#include "utils.h"

void parse_command(char *command){
    char **result = split(command,':');
    if (strcmp(result[0], "start") == 0)
    {
        printf("start!\n");
        STIM_START();
    }
    else if (strcmp(result[0], "stop") == 0)
    {
        printf("stop!\n");
        if (STIM_TASK_STATUS == 1)
        {
            STIM_STOP();
        }
    }else if(strcmp(result[0],"stim_amp")==0){
        STIM_AMP = atoi(result[1]);
    }
    else if (strcmp(result[0], "stim_type")==0)
    {
        STIM_TYPE = atoi(result[1]);
    }
    else if (strcmp(result[0], "anodic_cathodic")==0)
    {
        ANODIC_CATHODIC = atoi(result[1]);
    }
    else if (strcmp(result[0], "phase_one_time")==0)
    {
        PHASE_ONE_TIME = atoi(result[1]);
    }
    else if (strcmp(result[0], "inter_phase_gap")==0)
    {
        INTER_PHASE_GAP = atoi(result[1]);
    }
    else if (strcmp(result[0], "phase_two_time")==0)
    {
        PHASE_TWO_TIME = atoi(result[1]);
    }
    else if (strcmp(result[0], "inter_stim_delay")==0)
    {
        INTER_STIM_DELAY = atoi(result[1]);
    }
    else if (strcmp(result[0], "pulse_num")==0)
    {
        PULSE_NUM = atoi(result[1]);
    }
    else if (strcmp(result[0], "burst_num")==0)
    {
        BURST_NUM = atoi(result[1]);
    }
    else if (strcmp(result[0], "inter_burst_delay")==0)
    {
        INTER_BURST_DELAY = atoi(result[1]);
    }
    else if (strcmp(result[0], "ramp_up") == 0)
    {
        RAMP_UP = atoi(result[1]);
    }
    else if (strcmp(result[0], "short_electrode") == 0)
    {
        SHORT_ELECTRODE = atoi(result[1]);
    }
    else if (strcmp(result[0], "pulse_num_in_one_burst") == 0)
    {
        PULSE_NUM_IN_ONE_BURST = atoi(result[1]);
    }
    else if (strcmp(result[0], "enable_record") == 0)
    {
        ENABLE_RECORD = atoi(result[1]);
    }
    else if (strcmp(result[0], "record_offset") == 0)
    {
        RECORD_OFFSET = atoi(result[1]);
    }
    else if (strcmp(result[0], "debug_enable") == 0)
    {
        DEBUG_MODE_ENABLED = atoi(result[1]);
    }
    else if (strcmp(result[0], "dac_phase_one") == 0)
    {
        DAC_PHASE_ONE = atoi(result[1]);
    }
    else if (strcmp(result[0], "dac_phase_two") == 0)
    {
        DAC_PHASE_TWO = atoi(result[1]);
    }

     if(DEBUG_MODE_ENABLED){
        printf("***********************************************************\n");
        printf("%s and %s\n", ANODIC_CATHODIC ? "CATHODIC" : "ANODIC", STIM_TYPE ? "BURST" : "UNIFORM");
        printf("stim amp : %u   phase one time : %u    phase two time: %u\n", STIM_AMP, PHASE_ONE_TIME, PHASE_TWO_TIME);
        printf("inter phase gap : %u   inter stim delay : %u  pulse num : %u\n", INTER_PHASE_GAP, INTER_STIM_DELAY, PULSE_NUM);
        printf("pulse num in one burst : %u     burst num : %u    inter burst delay : %u\n", PULSE_NUM_IN_ONE_BURST,BURST_NUM, INTER_BURST_DELAY);
        printf("ramp up : %s\n",RAMP_UP?"yes":"no");
        printf("short electrode : %s\n",SHORT_ELECTRODE?"yes":"no");
        printf("recording : %s      recording start offset : %d\n", ENABLE_RECORD ? "yes" : "no", RECORD_OFFSET);
        printf("***********************************************************\n");
    }

    int x = 0;
    while(result[x] != NULL){
        free(result[x]);
        x++;
    }
    free(result);
}



char **split(const char *command, char splitter)
{
    char **result;
    int j, n = 0;
    int count = 1;
    int len = strlen(command);
    char tmp[len + 1];
    tmp[0] = '\0';

    for (int i = 0; i < len; ++i)
    {
        if (command[i] == splitter && command[i + 1] == '\0')
            continue;
        else if (command[i] == splitter && command[i + 1] != splitter)
            count++;
    }
    result = (char **)malloc((count + 1) * sizeof(char *));

    count = 0;
    for (int i = 0; i < len; ++i)
    {
        if (i == len - 1 && command[i] != splitter)
        {
            tmp[n++] = command[i];
            tmp[n] = '\0';
            j = strlen(tmp) + 1;
            result[count] = (char *)malloc(j * sizeof(char));
            strcpy(result[count++], tmp);
        }
        else if (command[i] == splitter)
        {
            j = strlen(tmp);
            if (j != 0)
            {
                tmp[n] = '\0';
                result[count] = (char *)malloc((j + 1) * sizeof(char));
                strcpy(result[count++], tmp);
                n = 0;
                tmp[0] = '\0';
            }
        }
        else
            tmp[n++] = command[i];
    }
    result[count] = NULL;
    return result;
}