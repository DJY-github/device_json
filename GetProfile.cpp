#include"GetProfile.h"

int str_to_int(char* str){
    int i;
    int n = 0;
    if (str[0] == '0' && (str[1]=='x' || str[1]=='X'))
    {
        i = 2;
    }
    else
    {
        i = 0;
    }
    for (; (str[i] >= '0' && str[i] <= '9') || (str[i] >= 'a' && str[i] <= 'z') || (str[i] >='A' && str[i] <= 'Z');++i)
    {
        if (tolower(str[i]) > '9')
        {
            n = 16 * n + (10 + tolower(str[i]) - 'a');
        }
        else
        {
            n = 16 * n + (tolower(str[i]) - '0');
        }
    }
    return n;
}

cJSON* OpenProfile(char *filename)
{
    cJSON* json;
    FILE *f;
	long len;
	char *data;

	f=fopen(filename,"rb");
    fseek(f,0,SEEK_END);//定义文件尾部
	len=ftell(f);

	fseek(f,0,SEEK_SET);//定义到文件头
	data=(char*)malloc(len+1);
	fread(data,1,len,f);
	fclose(f);

	json=cJSON_Parse(data);
	if (!json)
	{
        printf("Error before: [%s]\n",cJSON_GetErrorPtr());
    }

	free(data);
    return json;
}

Pro_Data GetProfileData(cJSON* json)
{
    int len=0;
    Pro_Data pro;
    Field data[BUFF_SIZE];

    cJSON *arrayItem = cJSON_GetObjectItem(json,"Address");
    pro.Address=arrayItem->valueint;

    arrayItem = cJSON_GetObjectItem(json,"Function");
    pro.Function=arrayItem->valueint;

    memset(data,0,sizeof(data));
    arrayItem = cJSON_GetObjectItem(json,"Data");
    for(int i=0;i<cJSON_GetArraySize(arrayItem);i++)
    {
        int addr;
        char *desc;
        cJSON *object;

        object= cJSON_GetArrayItem(arrayItem,i);
        cJSON *item = cJSON_GetObjectItem(object,"num");
        if(item->valueint<=0)
        {
            continue;
        }
        pro.Data[i].num=item->valueint;

        item = cJSON_GetObjectItem(object,"name");
        memcpy(&pro.Data[i].name,&item->valuestring,strlen(item->valuestring));
        item = cJSON_GetObjectItem(object,"addr");
        addr=str_to_int(item->valuestring);
        pro.Data[i].addr=addr;

        item = cJSON_GetObjectItem(object,"coeff");
        pro.Data[i].coeff=item->valuedouble;
        len+=1;
    }
    pro.len=len;
    return pro;
}

