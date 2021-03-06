# SoalShift_modul2_C02

## Soal
   1. Elen mempunyai pekerjaan pada studio sebagai fotografer. Suatu hari ada seorang klien yang bernama Kusuma yang meminta untuk mengubah nama file yang memiliki ekstensi .png menjadi “[namafile]_grey.png”. Karena jumlah file yang diberikan Kusuma tidak manusiawi, maka Elen meminta bantuan kalian untuk membuat suatu program C yang dapat mengubah nama secara otomatis dan diletakkan pada direktori /home/[user]/modul2/gambar. <br> Catatan : Tidak boleh menggunakan crontab.
   2. Pada suatu hari Kusuma dicampakkan oleh Elen karena Elen dimenangkan oleh orang lain. Semua kenangan tentang Elen berada pada file bernama “elen.ku” pada direktori “hatiku”. Karena sedih berkepanjangan, tugas kalian sebagai teman Kusuma adalah membantunya untuk menghapus semua kenangan tentang Elen dengan membuat program C yang bisa mendeteksi owner dan group dan menghapus file “elen.ku” setiap 3 detik dengan syarat ketika owner dan grupnya menjadi “www-data”. Ternyata kamu memiliki kendala karena permission pada file “elen.ku”. Jadi, ubahlah permissionnya menjadi 777. Setelah kenangan tentang Elen terhapus, maka Kusuma bisa move on. <br> Catatan: Tidak boleh menggunakan crontab
   3. Diberikan file campur2.zip. Di dalam file tersebut terdapat folder “campur2”. <br> Buatlah program C yang dapat : <br>
      i)  mengekstrak file zip tersebut. <br>
      ii) menyimpan daftar file dari folder “campur2” yang memiliki ekstensi .txt ke dalam file daftar.txt. <br>
      Catatan:  
      - Gunakan fork dan exec.
      - Gunakan minimal 3 proses yang diakhiri dengan exec.
      - Gunakan pipe
      - Pastikan file daftar.txt dapat diakses dari text editor
   4. Dalam direktori /home/[user]/Documents/makanan terdapat file makan_enak.txt yang berisikan daftar makanan terkenal di Surabaya. Elen sedang melakukan diet dan seringkali tergiur untuk membaca isi makan_enak.txt karena ngidam makanan enak. Sebagai teman yang baik, Anda membantu Elen dengan membuat program C yang berjalan setiap 5 detik untuk memeriksa apakah file makan_enak.txt pernah dibuka setidaknya 30 detik yang lalu (rentang 0 - 30 detik). <br> Jika file itu pernah dibuka, program Anda akan membuat 1 file makan_sehat#.txt di direktori /home/[user]/Documents/makanan dengan '#' berisi bilangan bulat dari 1 sampai tak hingga untuk mengingatkan Elen agar berdiet. <br>
      Contoh: <br>
      File makan_enak.txt terakhir dibuka pada detik ke-1<br>
      Pada detik ke-10 terdapat file makan_sehat1.txt dan makan_sehat2.txt<br>
      Catatan: 
      - dilarang menggunakan crontab
      - Contoh nama file : makan_sehat1.txt, makan_sehat2.txt, dst
   5. Kerjakan poin a dan b di bawah:<br>
      a. Buatlah program c untuk mencatat log setiap menit dari file log pada syslog ke /home/[user]/log/[dd:MM:yyyy-hh:mm]/log#.log<br>
      Ket:
        - Per 30 menit membuat folder /[dd:MM:yyyy-hh:mm]
        - Per menit memasukkan log#.log ke dalam folder tersebut<br>
          ‘#’ : increment per menit. Mulai dari 1<br>
          
      b. Buatlah program c untuk menghentikan program di atas. <br>
      NB: Dilarang menggunakan crontab dan tidak memakai argumen ketika menjalankan program.

## Jawaban
   1. Berikut adalah source code nya.
      ```
      #include <sys/types.h>
      #include <sys/stat.h>
      #include <stdio.h>
      #include <stdlib.h>
      #include <fcntl.h>
      #include <errno.h>
      #include <unistd.h>
      #include <syslog.h>
      #include <string.h>
      #include <dirent.h>

      int main() {
        pid_t pid, sid;

        pid = fork();

        if (pid < 0) {
          exit(EXIT_FAILURE);
        }

        if (pid > 0) {
          exit(EXIT_SUCCESS);
        }

        umask(0);

        sid = setsid();

        if (sid < 0) {
          exit(EXIT_FAILURE);
        }

        if ((chdir("/home/syauqi/modul2/")) < 0) {
          exit(EXIT_FAILURE);
        }

        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        while(1) {
          struct dirent *de;

          DIR *dr = opendir(".");

          if (dr == NULL)
          {
              printf("Could not open current directory" );
              return 0;
          }

          while ((de = readdir(dr)) != NULL)
          {
            char stre[256];
            char newpath[256];
            int i=0;
            snprintf(stre, 256, "%s", de->d_name);
            if(strstr(stre, ".png"))
            {
        for(i=0; i < sizeof(de->d_name); i++)
          if(stre[i]=='.') break;

        stre[i]='\0';
        snprintf(newpath, 256, "gambar/%s_grey.png", stre);
        rename(de->d_name, newpath);
            }
          }

          closedir(dr);
          sleep(30);
        }

        exit(EXIT_SUCCESS);
      }
      ```
      - Di sini kami menggunakan process daemon agar processnya terus berjalan setiap 30 detik.
      - Kami memakai library dirent.h agar dapat menggunakan data type struct dirent dan fungsi readdir().
      - Struct dirent nanti akan menyimpan nama file yang ada di folder yang telah ditentukan.
      - Dalam looping, setiap nama file yang mengandung ".png" akan diproses.
      - Pertama panjang dari nama file akan disimpan dalam variabel len, lalu dikurangi 4 untuk proses penghilangan ".png"nya
      - Lalu membuat variabel string dengan panjang len+9 karena "_grey.png" panjangnya 9.
      - Lalu nama file tadi di copy ke variabel newname dan ditambahkan _grey.png
      - Lalu dibuat variabel path untuk folder penyimpanan file yang akan dipindah
      - Lalu dengan fungsi rename untuk memindahkan filenya
      
      ![soal1](/images/soal1.png)
      
   2. Berikut adalah source codenya.
      ```
      #include <sys/types.h>
      #include <sys/stat.h>
      #include <stdio.h>
      #include <stdlib.h>
      #include <fcntl.h>
      #include <errno.h>
      #include <unistd.h>
      #include <syslog.h>
      #include <string.h>
      #include <errno.h>
      #include <pwd.h>
      #include <grp.h>

      #define die(e) do { fprintf(stderr, "%s\n", e); exit(EXIT_FAILURE); } while (0);

      int main(int argc, char **argv) {
        pid_t pid, sid;

        pid = fork();

        if (pid < 0) {
          exit(EXIT_FAILURE);
        }

        if (pid > 0) {
          exit(EXIT_SUCCESS);
        }

        umask(0);

        sid = setsid();

        if (sid < 0) {
          exit(EXIT_FAILURE);
        }

        if ((chdir("/home/syauqi/modul2/hatiku/")) < 0) {
          exit(EXIT_FAILURE);
        }

        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        while(1) {
          char mode[] = "0777";
          char buf[100] = "elen.ku";
          int i;
          i = strtol(mode, 0, 8);
          chmod (buf,i);

          struct stat info;
          stat(buf, &info);
          struct passwd *pw = getpwuid(info.st_uid);
          struct group *gr = getgrgid(info.st_gid);

          if(strcmp(pw->pw_name, "www-data")==0 && strcmp(gr->gr_name, "www-data")==0){
            int status;
            status = remove(buf);
          }

          sleep(3);
        }

        exit(EXIT_SUCCESS);
      }
      ```
      - Di sini kami menggunakan process daemon agar processnya terus berjalan dan diatur setiap 3 detik.
      - Pada prosesnya perta-tama kami mengubah permissions dari file dengan menggunakan chmod.
      - Kemudian mendeclare variabel struct stat yang berisi info dari file.
      - Lalu menggunakan function getweuid() dan getgrgid() untuk mendapatkan owner dan group dari file.
      - Kemudian di cek apakah owner dan group dari file tersebut adalah www-data atau bukan. Jika iya maka file akan dihapus.
      
      ![soal2](/images/soal2.png)
    
   3. Berikut adalah source codenya.
      ```
      #include <stdio.h>
      #include <stdlib.h>
      #include <unistd.h>
      #include <sys/wait.h>

      #define die(e) do { fprintf(stderr, "%s\n", e); exit(EXIT_FAILURE); } while (0);

      int main() {
        int link[2];
        pid_t pid;
        char foo[4096];
        int status;
        int link2[2];
        if (pipe(link2)==-1)
          die("pipe");

        if (pipe(link)==-1)
          die("pipe");

        if ((pid = fork()) == -1)
          die("fork");

        if(pid == 0) {

          execl("/usr/bin/unzip", "unzip", "/home/syauqi/modul2/campur2.zip", (char *)0);
          die("execl");

        } else {
          while((wait(&status))>0);
          int statu;

          if ((pid = fork()) == -1)
          die("fork");

          if (pid == 0) {
            dup2 (link[1], STDOUT_FILENO);
            close(link[0]);
            close(link[1]);
            execl("/bin/ls", "ls", "/home/syauqi/modul2/campur2/", (char *)0);
            die("execl");
          }

          else {
            if ((pid = fork()) == -1)
              die("fork");

            if (pid == 0) {
              dup2 (link[0], STDIN_FILENO);
              dup2 (link2[1], STDOUT_FILENO);
              close(link[1]);
              close(link[0]);
         close(link2[1]);
              close(link2[0]);
              execl("/bin/grep", "grep", ".txt$", (char *)0);
              die("execl");
            }

            else{
              FILE *f = fopen("daftar.txt", "w");
              close(link2[1]);
         close(link[1]);
         close(link[0]);
              int nbytes = read(link2[0], foo, sizeof(foo));
              fprintf(f, "%.*s\n", nbytes, foo);
              fclose(f);
            }
          }
        }
        return 0;
      }
      ```
      - Disini kami menggunakan 2 pipe dan 3 child process.
      - Process pertama berfungsi untup melakukan unzip file.
      - Process kedua melakukan exec untuk menjalankan perintah ls, yang nanti outputnya disimpan dalam pipe pertama (link). Fungsi ls disini untuk mendapatkan nama-nama file dari hasil unzip sebelumnya.
      - Process ketiga melakukan exec untuk menjalankan perintah grep, yang mendapatkan input dari pipe pertama (link) dan outputnya nanti akan disimpan dalam pipe kedua (link2). Fungsi grep disini untuk mensortir file yang memiliki ekstensi .txt.
      - Process keempat melakukan print ke dalam file, yang mendapatkan input dari pipe kedua (link2).
      
      ![soal3](/images/soal3.png)
   
   4. Berikut adalah source codenya.
      ```
      #include <sys/types.h>
      #include <sys/stat.h>
      #include <stdio.h>
      #include <stdlib.h>
      #include <fcntl.h>
      #include <errno.h>
      #include <unistd.h>
      #include <syslog.h>
      #include <string.h>
      #include <time.h>

      static time_t getFileAccessedTime(const char *path)
      {
         struct stat attr;
         if (stat(path, &attr) == 0)
         {
            return attr.st_atime;
         }
         return 0;
      }

      int i = 1;

      int main() {
      pid_t pid, sid;

      pid = fork();

      if (pid < 0) {
         exit(EXIT_FAILURE);
      }

      if (pid > 0) {
         exit(EXIT_SUCCESS);
      }

      umask(0);

      sid = setsid();

      if (sid < 0) {
         exit(EXIT_FAILURE);
      }

      if ((chdir("/")) < 0) {
         exit(EXIT_FAILURE);
      }

      close(STDIN_FILENO);
      close(STDOUT_FILENO);
      close(STDERR_FILENO);

      while(1) {
         time_t t1 = getFileAccessedTime("/home/becak/Documents/makanan/makan_enak.txt");
         time_t t2 = time(NULL);
         char filename [FILENAME_MAX];

         if (t2 - t1 <= 30) {
            sprintf(filename, "/home/becak/Documents/makanan/makan_sehat%d.txt", i);
            FILE *new_file;
            new_file = fopen(filename,"w");
            fclose(new_file);
           i++;
         }

      sleep(5);
      }

      exit(EXIT_SUCCESS);
      }
      ```
      - Untuk mengetahui waktu terkahir kali file dibuka maka dibuat sebuah fungsi yang memanfaatkan library `time.h` menggunakan fungsi `st_atime`.
         ```
         static time_t getFileAccessedTime(const char *path)
         {
            struct stat attr;
            if (stat(path, &attr) == 0)
            {
               return attr.st_atime;
            }
            return 0;
         }
         ```
      - Setelah diketahui maka waktu sistem kita kurangi dengan waktu akses terakhir kali file tersebut, apabila selisih tersebut kurang dari sama dengan 30 detik maka kita buat makan_sehat#.txt dengan mengincrement index file tersebut.
         ```
         while(1) {
         time_t t1 = getFileAccessedTime("/home/becak/Documents/makanan/makan_enak.txt");
         time_t t2 = time(NULL);
         char filename [FILENAME_MAX];

         if (t2 - t1 <= 30) {
            sprintf(filename, "/home/becak/Documents/makanan/makan_sehat%d.txt", i);
            FILE *new_file;
            new_file = fopen(filename,"w");
            fclose(new_file);
           i++;
         }
         ```
       - Setting program agar berjalan tiap 5 detik menggunakan `sleep(5);`.
       - Output:
       ![soal4](/images/soal4.png)
       
   5. Berikut adalah source codenya.
      ```
      #include <sys/types.h>
      #include <sys/stat.h>
      #include <stdio.h>
      #include <stdlib.h>
      #include <fcntl.h>
      #include <errno.h>
      #include <unistd.h>
      #include <syslog.h>
      #include <string.h>
      #include <time.h>

      int i = 30;

      int main() {
        pid_t pid, sid;
        FILE *pid_txt;

        pid = fork();

        if (pid < 0) {
          exit(EXIT_FAILURE);
        }

        if (pid > 0) {
          pid_txt = fopen("/home/becak/modul2/pid_nomor5.txt","w");
          fprintf(pid_txt, "%d", pid);
          fclose(pid_txt);
          exit(EXIT_SUCCESS);
        }

        umask(0);

        sid = setsid();

        if (sid < 0) {
          exit(EXIT_FAILURE);
        }

        if ((chdir("/")) < 0) {
          exit(EXIT_FAILURE);
        }

        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        while(1) {
          char filename[FILENAME_MAX];
          char directory[FILENAME_MAX];
          char appendlog[100];
          char text[999999];
          FILE *syslog, *loglog;
          int i = 30;

          if (i == 30){
              struct tm *timenow;
              time_t now = time(NULL);
              timenow = localtime(&now);
              strftime(directory, sizeof(directory), "/home/becak/log/%d:%m:%Y-%H:%M", timenow);
              mkdir(directory, ACCESSPERMS);
              i = 0;
          }

          sprintf(appendlog, "/log%d.log", i);
          strcpy(filename, directory);
          strcat(filename, appendlog);
          syslog = fopen("/var/log/syslog","r");
          loglog = fopen(filename, "w");
          while((fgets(text, sizeof(text), syslog)) != NULL) {
              fputs(text, loglog);
          }

          fclose(syslog);
          fclose(loglog);
          i++;


          sleep(60);
        }

        exit(EXIT_SUCCESS);
      }
      ```

      - Deklarasi variabel global i = 30, nantinya variabel i ini berguna untuk pembuatan direktori dan penamaan index file.
      - Pada pertama kali eksekusi maka program akan membuat direktori dalam /home/becak/log karena i = 30.
      - Penamaan direktori berdasarkan waktu sistem dengan format "dd:mm:YYYY-HH:MM" memanfaatkan library `time.h`.
      - Setelah direktori dibuat maka set i = 0 agar nantinya digunakan untuk penamaan index file log.
      - Buat file berupa log'i'.log. Karena i diincrement, maka nantinya index dari file tersebut akan bertambah.
      - Set sleep(60), artinya program akan berjalan tiap menit. Nantinya pada saat i = 30 atau 30 menit maka akan membuat direktori lagi pada /home/becak/log.
      
       ![soal5](/images/soal5_a_1.png)
       ![soal5](/images/soal5_a_2.png)
   
   b. Berikut adalah source codenya
   
      ```
      #include <stdio.h>
      #include <unistd.h>
      #include <sys/types.h>
      #include <signal.h>
      
      int main()
      {
        FILE *pidnya;
        pidnya = fopen("/home/becak/modul2/pid_nomor5.txt", "r");
        int pid_kill;
        
        fscanf(pidnya, "%d", &pid_kill);
        
        kill(pid_kill, SIGKILL);
        
        fclose(pidnya);
        return 0;
      }
      ```
   - pada soal5a terdapat syntax yang menyimpan pidnya di file pid_nomor5.txt.
   - lalu dibaca oleh program 5b dan kill
   ![soal5](/images/soal5_b.png)


# Terima Kasih :)
