// #include <stdio.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_system.h"
// #include "nvs_flash.h"
// #include "nvs.h"

// void app_main(void)
// {
//     while (1)
//     {
//             esp_err_t err = nvs_flash_init();
//         if(err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
//         {
//             ESP_ERROR_CHECK(nvs_flash_erase());
//             err = nvs_flash_init();
//         }
//         ESP_ERROR_CHECK(err);

//     //Write
//     printf("\n");
//     nvs_handle_t my_handle;
//     err = nvs_open("storage", NVS_READWRITE, &my_handle);
//     uint8_t Buf_Write[10];
//     for (int i = 0; i < 10; i++)
//     {
//         Buf_Write[i] = 'a' + i;
//     }
//     err = nvs_set_blob(my_handle, "GIA TRI DEM", Buf_Write, sizeof(Buf_Write));
//     err = nvs_commit(my_handle);
//     if ((err == ESP_OK))
//     {
//         printf("Ghi thanh  cong!\n");
//     }
//     else
//     {
//         printf("Ghi that bai\n");
//     }
//     nvs_close(my_handle);

//     //Read
//     err = nvs_open("storage", NVS_READWRITE, &my_handle);
//     uint8_t Buf_Read[10];
//     size_t size = sizeof(Buf_Read);
//     err = nvs_get_blob(my_handle, "GIA TRI DEM", Buf_Read, &size);
//     nvs_close(my_handle);

//     //Printf
//     printf("\n");
//     for (int i = 0; i < 10; i++)
//     {
//         printf("%c\n", Buf_Read[i]);
//     }
//     vTaskDelay(2000 / portTICK_PERIOD_MS);
//     }
// }






#include <stdio.h>
#include "esp_system.h"
#include "esp_partition.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

uint16_t a = 0;

void app_main(void)
{
    while (1)
    {
        a++;
        // uint8_t *data_r, *data_w;      
        uint8_t data_r[15], data_w[15];                                        // con trỏ tới vùng nhớ cần đọc
        size_t size = 15;                             // kích thước của vùng nhớ cần đọc
        esp_err_t err;                                                // biến để kiểm tra lỗi

        const esp_partition_t *partition = esp_partition_find_first(
        ESP_PARTITION_TYPE_DATA , ESP_PARTITION_SUBTYPE_ANY, NULL);
        if (partition == NULL) {
            printf("Không tìm thấy phân vùng\n");
            return;
        }


        for (int i = 0; i < 15; i++)
        {
            data_w[i] = i + a;
            printf("%d\t", data_w[i]);
        }
        
        printf("\n");

        // xóa dữ liệu từ một vị trí bắt đầu (start address) cho đến một vị trí kết thúc (end address) 
        err = esp_partition_erase_range(partition, 0x1000, 1023);
        if (err != ESP_OK) {
            printf("Lỗi khi xóa dữ liệu cũ: %s\n", esp_err_to_name(err));
            // free(data_w);
        }

        //Điền dữ liệu vào bộ nhớ Flash
        err = esp_partition_write(partition, 0x1000, data_w, size);
        if (err != ESP_OK) {
            printf("Lỗi khi ghi dữ liệu: %s\n", esp_err_to_name(err));
            // free(data_w);
        }

        //Khởi tạo biến ghi
        // data_r = malloc(size);
        // if (data_r == NULL) {
        //     printf("Không đủ bộ nhớ để cấp phát\n");
        //     return;
        // }

         //Đoc dữ liệu vào bộ nhớ Flash
        err = esp_partition_read(partition, 0x1000, data_r, size);
        if (err != ESP_OK) {
            printf("Lỗi khi đọc dữ liệu: %s\n", esp_err_to_name(err));
            // free(data_r);
        }
        else
        {
            for (int i = 0x1000; i < 0x100f; i++)
            {
                printf("%d\t", data_r[i]);
            }
            // free(data_r);
        }
        printf("\n");
        printf("\n");
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }

}




// #include "esp_system.h"
// #include "esp_partition.h"

// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"

// #include <stdio.h>
// #include <stdlib.h>
// #include "esp_err.h"
// #include "esp_flash.h"

// void app_main(void)
// {
//     uint32_t start_addr = 0x1000;
//     uint32_t end_addr = 0xffff;
//     uint32_t data_size = end_addr - start_addr + 1;

//     uint32_t *data_read = (uint32_t *)malloc(data_size); // con trỏ để lưu trữ dữ liệu khi đọc
//     uint32_t *data_write = (uint32_t *)malloc(data_size); // con trỏ để lưu trữ dữ liệu khi ghi

//     // Khởi tạo dữ liệu để ghi vào Flash
//     for (uint32_t i = 0; i < data_size; i++) {
//         *(data_write + i) = i + 1;
//     }

//     
//     esp_flash_t* flash_chip = NULL;
//     esp_err_t err = esp_flash_init(flash_chip);
//     if (err == ESP_OK) {
//         printf("Khoi tao thanh cong\n");
//     }

//     // Ghi dữ liệu vào Flash
//     esp_err_t ret = esp_flash_write(flash_chip, data_write, start_addr, data_size);;
//     if (ret != ESP_OK) {
//         printf("Lỗi khi ghi dữ liệu vào Flash: %s\n", esp_err_to_name(ret));
//         free(data_read);
//         free(data_write);
      
//     }

//     // Đọc dữ liệu từ Flash
//     ret = esp_flash_read(flash_chip, data_read, start_addr, data_size);
   
//     if (ret != ESP_OK) {
//         printf("Lỗi khi đọc dữ liệu từ Flash: %s\n", esp_err_to_name(ret));
//         free(data_read);
//         free(data_write);
     
//     }

//     // In ra dữ liệu đọc được
//     for (uint32_t i = 0; i < data_size; i++) {
//         printf("Địa chỉ 0x%x: %d\n", start_addr + i * 4, *(data_read + i));
//     }

//     free(data_read);
//     free(data_write);
// }