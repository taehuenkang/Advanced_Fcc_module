
#include <stdio.h>
#include "xparameters.h"
#include "xil_io.h"
#include "xtime_l.h"  // To measure of processing time
#include <stdlib.h>	  // To generate rand value
#include <assert.h>

#define DATA_GEN 1
#define SW_RUN 2
#define HW_RUN 3
#define CHECK 4

#define AXI_DATA_BYTE 4

#define IDLE 1
#define RUN 1 << 1
#define DONE 1 << 2

#define CTRL_REG 0
#define STATUS_REG 1
#define MEM0_ADDR_REG 2
#define MEM0_DATA_REG 3
#define MEM1_ADDR_REG 4
#define MEM1_DATA_REG 5
#define RESULT_0_REG 6
#define RESULT_1_REG 7
#define RESULT_2_REG 8
#define RESULT_3_REG 9

#define MEM_DEPTH 4096 
#define NUM_CORE 4

int main() {
    int data;
    int case_num;
    int read_data;
    XTime tStart, tEnd;
    volatile int i, core;
    signed int* write_buf_node;
    write_buf_node = (signed int*)malloc(sizeof(signed int) * MEM_DEPTH);

    signed int* write_buf_wegt;
    write_buf_wegt = (signed int*)malloc(sizeof(signed int) * MEM_DEPTH);

    signed char IN_NODE[NUM_CORE]; // 8b
    signed char IN_WEGT[NUM_CORE]; // 8b
    signed char bias[NUM_CORE];    //16b
    signed int  OT_RSLT_SW[NUM_CORE] = { 0, }; // 32b init 0
    signed int  OT_RSLT_HW[NUM_CORE] = { 0, }; // 32b init 0
    // ReLU 함수 정의
    int relu(int x) {
        return (x > 0) ? x : 0;
    }

    while (1) {
        printf("======= Hello World ======\n");
        printf("plz input run mode\n");
        printf("1. DATA GEN \n");
        printf("2. SW RUN \n");
        printf("3. HW RUN \n");
        printf("4. CHECK SW vs HW result\n");

        scanf("%d", &case_num);

        if (case_num == DATA_GEN) {

            for (i = 0; i < MEM_DEPTH; i++) {
                write_buf_node[i] = 0; // init
                write_buf_wegt[i] = 0; // init
                for (core = 0; core < NUM_CORE; core++) {
                    IN_NODE[core] = rand() % 256; // 0~255 8b
                    IN_WEGT[core] = rand() % 256; // 0~255 8b
                    bias[core] = rand() % 65536; // 0~65536 16b (2의16승)
                }
                write_buf_node[i] |= IN_NODE[0]; write_buf_node[i] = write_buf_node[i] << 8;
                write_buf_node[i] |= IN_NODE[1]; write_buf_node[i] = write_buf_node[i] << 8;
                write_buf_node[i] |= IN_NODE[2]; write_buf_node[i] = write_buf_node[i] << 8;
                write_buf_node[i] |= IN_NODE[3];
                write_buf_wegt[i] |= IN_WEGT[0]; write_buf_wegt[i] = write_buf_wegt[i] << 8;
                write_buf_wegt[i] |= IN_WEGT[1]; write_buf_wegt[i] = write_buf_wegt[i] << 8;
                write_buf_wegt[i] |= IN_WEGT[2]; write_buf_wegt[i] = write_buf_wegt[i] << 8;
                write_buf_wegt[i] |= IN_WEGT[3];
            }
            printf("Done Data gen \n");
        }
        else if (case_num == SW_RUN) {
            for (core = 0; core < NUM_CORE; core++) {
                OT_RSLT_SW[core] = 0;
            }

            XTime_GetTime(&tStart);
            for (i = 0; i < MEM_DEPTH - 1; i++) {

                IN_NODE[0] = write_buf_node[i] >> 24;
                IN_NODE[1] = write_buf_node[i] >> 16;
                IN_NODE[2] = write_buf_node[i] >> 8;
                IN_NODE[3] = write_buf_node[i];
                IN_WEGT[0] = write_buf_wegt[i] >> 24;
                IN_WEGT[1] = write_buf_wegt[i] >> 16;
                IN_WEGT[2] = write_buf_wegt[i] >> 8;
                IN_WEGT[3] = write_buf_wegt[i];
                // Cal
                for (core = 0; core < NUM_CORE; core++) {
                    OT_RSLT_SW[core] += relu(IN_NODE[core] * IN_WEGT[core]);
                }
            }

            if (i == MEM_DEPTH - 1) {
                // bias 추가
                for (core = 0; core < NUM_CORE; core++) {
                    OT_RSLT_SW[core] = OT_RSLT_SW[core] + bias[core];
                    // ReLU 적용
                    OT_RSLT_SW[core] = relu(OT_RSLT_SW[core]);
                }

            }


            XTime_GetTime(&tEnd);
            printf("SW Done\n");
            printf("Output took %llu clock cycles.\n", 2 * (tEnd - tStart));
            printf("Output took %.2f us.\n",
                1.0 * (tEnd - tStart) / (COUNTS_PER_SECOND / 1000000));
        }
        else if (case_num == HW_RUN) {
            double hw_processing_time = 0.0;

            Xil_Out32((XPAR_TOP_DATA_MOVER_0_BASEADDR)+(CTRL_REG * AXI_DATA_BYTE), (u32)(0)); // init core ctrl reg
            Xil_Out32((XPAR_TOP_DATA_MOVER_0_BASEADDR)+(MEM0_ADDR_REG * AXI_DATA_BYTE), (u32)(0x00000000)); // Clear
            // Data Loading to BRAM 0
            for (i = 0; i < MEM_DEPTH - 1; i++) {
                Xil_Out32((XPAR_TOP_DATA_MOVER_0_BASEADDR)+(MEM0_DATA_REG * AXI_DATA_BYTE), write_buf_node[i]);
            }

            printf("BRAM 0 Write Done\n");

            // Data Loading to BRAM 1
            Xil_Out32((XPAR_TOP_DATA_MOVER_0_BASEADDR)+(MEM1_ADDR_REG * AXI_DATA_BYTE), (u32)(0x00000000)); // Clear
            for (i = 0; i < MEM_DEPTH; i++) {
                Xil_Out32((XPAR_TOP_DATA_MOVER_0_BASEADDR)+(MEM1_DATA_REG * AXI_DATA_BYTE), write_buf_wegt[i]);
            }

            printf("BRAM 1 Write Done\n");

            // Cal
            XTime_GetTime(&tStart);
            // check IDLE
            do {
                read_data = Xil_In32((XPAR_TOP_DATA_MOVER_0_BASEADDR)+(STATUS_REG * AXI_DATA_BYTE));
            } while ((read_data & IDLE) != IDLE);
            Xil_Out32((XPAR_TOP_DATA_MOVER_0_BASEADDR)+(CTRL_REG * AXI_DATA_BYTE), (u32)(MEM_DEPTH | 0x80000000)); // MSB run
            // wait done
            do {
                read_data = Xil_In32((XPAR_TOP_DATA_MOVER_0_BASEADDR)+(STATUS_REG * AXI_DATA_BYTE));
            } while ((read_data & DONE) != DONE);
            XTime_GetTime(&tEnd);
            printf("FC Core Done\n");
            printf("Output took %llu clock cycles.\n", 2 * (tEnd - tStart));
            printf("Output took %.2f us.\n",
                1.0 * (tEnd - tStart) / (COUNTS_PER_SECOND / 1000000));
            hw_processing_time += 1.0 * (tEnd - tStart) / (COUNTS_PER_SECOND / 1000000);

            // Write Buffer from bram1
            OT_RSLT_HW[0] = Xil_In32((XPAR_TOP_DATA_MOVER_0_BASEADDR)+(RESULT_0_REG * AXI_DATA_BYTE)) + bias[0];   //bias추가
            OT_RSLT_HW[1] = Xil_In32((XPAR_TOP_DATA_MOVER_0_BASEADDR)+(RESULT_1_REG * AXI_DATA_BYTE)) + bias[1];
            OT_RSLT_HW[2] = Xil_In32((XPAR_TOP_DATA_MOVER_0_BASEADDR)+(RESULT_2_REG * AXI_DATA_BYTE)) + bias[2];
            OT_RSLT_HW[3] = Xil_In32((XPAR_TOP_DATA_MOVER_0_BASEADDR)+(RESULT_3_REG * AXI_DATA_BYTE)) + bias[3];

            printf("Result Read Done\n");

        }
        else if (case_num == CHECK) {
            for (i = 0; i < NUM_CORE; i++) {
                if (OT_RSLT_SW[i] != OT_RSLT_HW[i]) {  // Check Result
                    printf("Mismatch!! idx : %d, OT_RSLT_SW : %d, OT_RSLT_HW : %d\n", i, OT_RSLT_SW[i], OT_RSLT_HW[i]);
                }
            }
            printf("Success.\n");
        }
        else {
            // no operation, exit
            //break;
        }
    }
    free(write_buf_node);
    free(write_buf_wegt);
    return 0;
}
