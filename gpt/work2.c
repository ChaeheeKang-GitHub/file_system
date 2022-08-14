#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <netinet/in.h> 
#include <memory.h>

//Sector2~33 Entry 1~128
typedef struct gpt_entry
{
    uint64_t partion_type_firstguid;
    uint64_t partion_type_lasttguid;
    uint64_t uinque_partion_firstguid;
    uint64_t uinque_partion_lasstguid;
    uint64_t first_lba;
    uint64_t last_lba;
    uint64_t attr_flag;
    uint16_t partion_name[72/sizeof (uint16_t)];
}__attribute__ ((packed)) gpt_entry;

int main(int arc, char* argv[]){
    FILE *fp = fopen(argv[1],"rb");

    if (fp==0)
    {
        printf("There is no %s \n",argv[1]);
        return 0;
    }
    //gpt_guid *g_guid=(gpt_guid*)malloc(sizeof (gpt_guid));


    for(int i=0;i<128;i++){
        //구조체 포인터에 메모리 할당
        gpt_entry *g_ent=(gpt_entry*)malloc(sizeof (gpt_entry));

        fseek(fp,1024+128*i,SEEK_SET);
        fread(g_ent,sizeof (g_ent)*16,1,fp);

        if(g_ent->first_lba==0) continue;
        else{
            //guid
            long int firstguid = ntohl(g_ent->partion_type_firstguid>>32)|((uint64_t)ntohl(g_ent->partion_type_firstguid)<<32);
            long int lasttguid = ntohl(g_ent->partion_type_lasttguid>>32)|((uint64_t)ntohl(g_ent->partion_type_lasttguid)<<32);
            
            printf("%lx%lx ",firstguid,lasttguid);

            //실제 real offset sector
            printf("%ld ",g_ent->first_lba*0x200);
            //사이즈
            printf("%ld \n",(g_ent->last_lba - g_ent->first_lba +1)*0x200);
        }
    }

    fclose(fp);
    return 0;
}
