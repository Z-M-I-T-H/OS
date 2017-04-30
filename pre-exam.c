#define _XOPEN_SOURCE 700
#include<ftw.h>
#include<dirent.h>
#include<grp.h>
#include<time.h>
#include<pwd.h>
#include<utime.h>
#include<sys/time.h>
#include <sys/types.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<unistd.h>

#include<stdbool.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>

#define MAX 256
#define SEKUNDI_U_DANU (3600*24)
#define UNUSED(x) ((void)x)
#define ma_greska(uslov, poruka)\
	do{\
		if(!(uslov))\
			ma_greska_impl(poruka, __FILE__, __func__, __LINE__);\
	}while(0);

void ma_greska_impl(const char* poruka, const char* ime_fajla, const char* ime_funk, const int linija);

bool fajl_open(const char* putanja, const char* rezim, int* fd);
bool katze(const int fd, const int fd2);
bool kopi_fajl(const char* p1, const char *p2);

void skrati_fajl(const char* p);

void listaj_korisnike();
bool daj_info_korisnika(const char *ime, FILE *f);

void daj_info_fajla(const char *p, char** info_fajla);
char* daj_ime_korisnika(const uid_t id);
char *daj_ime_grupe(const gid_t id);

bool izmeri_velicinu_foldera(const char *p, uintmax_t* size);

uintmax_t osDays = 0;
char *OsExt = NULL;
int daj_info_menjanja (const char *fpath, const struct stat *sb,
                          int typeflag, struct FTW *ftwbuf);
int daj_fajl_ext(const char *fpath, const struct stat *sb,
                          int typeflag, struct FTW *ftwbuf);

int main(int argc, char** argv){
	ma_greska(argc == 4, "Br. arg.");
// 	int velicina_fajla;
// 	ma_greska((velicina_fajla = lseek(fd, 0, SEEK_END)) >= 0, "SIZE");
// 	printf("velicina_fajla : %jdB\n", (uintmax_t)velicina_fajla);
// 	ma_greska(unlink(argv[1]) == 0, "WIPINGFILE");
// 	ma_greska(mkdir(argv[1], (mode_t)0644) == 0, "MAKINGFOLDER");
// 	ma_greska(rmdir(argv[1]) == 0, "WIPINGFOLDER");
	
	
// 	ma_greska(kopi_fajl(argv[1], argv[2]), "CAT");

	
// 	skrati_fajl(argv[1]);
	
	
// 	listaj_korisnike();
// 	char *info_fajla = NULL;
// 	daj_info_fajla(argv[1], &info_fajla);
// 	puts(info_fajla);
// 	
	
// 	ma_greska(chmod(argv[1], strtol(argv[2], NULL, 8)) != -1, "CHANGING MODE");
	
	
// 	int fd;
// 	mode_t old_umask = umask(strtol(argv[2], NULL, 8));
// 	ma_greska(fajl_open(argv[1], "w", &fd), "OPEN");
// 	umask(old_umask);
	
	
// 	daj_info_fajla(argv[1], &info_fajla);
// 	puts(info_fajla);
// 	free(info_fajla);
	
	
// 	struct stat finfo;
// 	ma_greska(stat(argv[1], &finfo) != -1, "CATCH INFO");
// 	ma_greska(S_ISDIR(finfo.st_mode), "NOT FOLDER");
// 	uintmax_t velicina_foldera = 0;
	
// 	ma_greska(izmeri_velicinu_foldera(argv[1], &velicina_foldera), "MESSURING");
// 	printf("%jdB\n", velicina_foldera);
	
	osDays = atoi(argv[2]);
	OsExt = argv[3];
	
	ma_greska(nftw(argv[1], daj_info_menjanja, 50, 0) != -1, "REVIEWING OF FOLDERS");
	ma_greska(nftw(argv[1], daj_fajl_ext, 50, 0) != -1, "REVIEWING OF FOLDERS");
	
	return 0;
}


void ma_greska_impl(const char* poruka, const char* ime_fajla, const char* ime_funk, const int linija){
	fprintf(stderr, "[%s] %s : %d\n", ime_fajla, ime_funk, linija);
	perror(poruka);
	puts(strerror(errno));
	exit(EXIT_FAILURE);
}

bool fajl_open(const char* putanja, const char* rezim, int* fd){
	int flegovi = 0;
	
	static mode_t privilegije = 0666; // privilegije novog fajla
	
	switch(rezim[0]){
		case 'r':
			flegovi |= (rezim[1] == '+') ? O_RDWR : O_RDONLY;
			break;
		case 'w':
			flegovi |= (rezim[1] == '+') ? O_RDWR : O_WRONLY;
			flegovi |= O_TRUNC;
			flegovi |= O_CREAT;
			break;
		case 'a':
			flegovi |= (rezim[1] == '+') ? O_RDWR : O_WRONLY;
			flegovi |= O_APPEND;
			flegovi |= O_CREAT;
			break;
		default:
			return false;
			break;
	}
		
	*fd = open(putanja, flegovi, privilegije);
	return *fd != -1;
}

bool katze(const int fd, const int fd2){
	size_t velicina_magacina = 1 << 13U;
	char *magacin = malloc(velicina_magacina);
	if(magacin == NULL) return false;
	ssize_t read_bytes;
	while((read_bytes = read(fd, magacin, velicina_magacina)) > 0)
		if(write(fd2, magacin, read_bytes) == -1){
			free(magacin);
			return false;
		}
	return true;
}

bool kopi_fajl(const char* p1, const char *p2){
	int fd, fd2;
	ma_greska(fajl_open(p1, "r", &fd), "OPENING");
	ma_greska(fajl_open(p2, "w+", &fd2), "OPENING");
	
	ma_greska(katze(fd, fd2), "CAT");
	close(fd2);
	close(fd);
	return true;
}

void skrati_fajl(const char* p){
	struct stat finfo;
	ma_greska(stat(p, &finfo) != -1, "CATCH INFO");
	FILE *f;
	ma_greska((f = fopen(p, "w")) != NULL, "Skracivanje neuspesno");
	fclose(f);
	
	struct utimbuf times;
	times.actime = finfo.st_atim.tv_sec;
	times.modtime = finfo.st_mtim.tv_sec;
	ma_greska((utime(p, &times) == 0), "Modifikacija vremena neuspela");
}

void listaj_korisnike(){
	struct passwd *korisnik;
	setpwent();
	while((korisnik = getpwent()) != NULL){
		daj_info_korisnika(korisnik->pw_name, stdout);
	}
	endpwent();
}

bool daj_info_korisnika(const char *ime, FILE *f){
	struct passwd *korisnik = getpwnam(ime);
	ma_greska(korisnik != NULL, "CATCH INFO FROM NAME");
	// sve iz /etc/passwd 
	fputs(korisnik->pw_name,f);
	fputs(korisnik->pw_passwd,f);
	fprintf(f, "%jd\n", (uintmax_t)korisnik->pw_uid);
	fprintf(f, "%jd\n", (uintmax_t)korisnik->pw_gid);
	fputs(korisnik->pw_gecos,f);
	fputs(korisnik->pw_dir,f);
	fputs(korisnik->pw_shell,f);
	
	return ferror(f) == 0;
}

void daj_info_fajla(const char *p, char** info_fajla){
	static const char* privilegije = "-rwxrwxrwx";
	errno = EINVAL;
	*info_fajla = malloc(MAX);
	ma_greska(*info_fajla != NULL, "Alociranje");
	
	char *finfo_string = *info_fajla;
	
	strcpy(finfo_string, privilegije);
	struct stat finfo;
	ma_greska(stat(p, &finfo) != -1, "CATCH INFO");
	
	switch(finfo.st_mode & S_IFMT){
		case S_IFSOCK:
			finfo_string[0] = 's';
			break;
		case S_IFLNK:
			finfo_string[0] = 'l';
			break;
		case S_IFREG:
			finfo_string[0] = '-';
			break;
		case S_IFBLK:
			finfo_string[0] = 'b';
			break;
		case S_IFDIR:
			finfo_string[0] = 'd';
			break;
		case S_IFCHR:
			finfo_string[0] = 'c';
			break;
		case S_IFIFO:
			finfo_string[0] = 'f';
			break;
	}
	
	
	if((finfo.st_mode & S_IRUSR) == 0) finfo_string[1] = '-';
	if((finfo.st_mode & S_IWUSR) == 0) finfo_string[2] = '-';
	if((finfo.st_mode & S_IXUSR) == 0) finfo_string[3] = '-';
	if((finfo.st_mode & S_IRGRP) == 0) finfo_string[4] = '-';
	if((finfo.st_mode & S_IWGRP) == 0) finfo_string[5] = '-';
	if((finfo.st_mode & S_IXGRP) == 0) finfo_string[6] = '-';
	if((finfo.st_mode & S_IROTH) == 0) finfo_string[7] = '-';
	if((finfo.st_mode & S_IWOTH) == 0) finfo_string[8] = '-';
	if((finfo.st_mode & S_IXOTH) == 0) finfo_string[9] = '-';
	
	finfo_string = finfo_string + (intptr_t)strlen(privilegije);
	finfo_string += sprintf(finfo_string, " %jd", (uintmax_t)finfo.st_nlink);
	char *usrnam = daj_ime_korisnika(finfo.st_uid);
	ma_greska(usrnam != NULL, "CATCH USRNM");
	finfo_string += sprintf(finfo_string, " %s", usrnam);
	free(usrnam);
	usrnam = daj_ime_grupe(finfo.st_uid);
	ma_greska(usrnam != NULL, "CATCH GRPNM");
	finfo_string += sprintf(finfo_string, " %s", usrnam);
	free(usrnam);
	time_t sekunde = finfo.st_mtime;
	finfo_string += sprintf(finfo_string, " %s", ctime(&sekunde));
	finfo_string += sprintf(finfo_string - (intptr_t)1, " %s\n", p);
	
}

char* daj_ime_korisnika(const uid_t id){
	struct passwd *korisnik = getpwuid(id);
	ma_greska(korisnik != NULL,"greska");
	char *usrnam = malloc(MAX);
	ma_greska(usrnam != NULL, "greska");
	strcpy(usrnam, korisnik->pw_name);
	return usrnam;
}

char *daj_ime_grupe(const gid_t id){
	struct group *gr = getgrgid(id);
	ma_greska(gr != NULL,"greska");
	char *group = malloc(MAX);
	ma_greska(group != NULL, "greska");
	strcpy(group, gr->gr_name);
	return group;	
}


bool izmeri_velicinu_foldera(const char *p, uintmax_t* size){ // *size kada se prosledi mora biti inicijalizovan!
	struct stat finfo;
	if(stat(p, &finfo) == -1) return false;
	*size += finfo.st_size;
	if(!S_ISDIR(finfo.st_mode)) return false;
	
	DIR* d = opendir(p);
	if(d == NULL) return false;
	
	struct dirent *en = NULL;
	char *new_p = malloc(MAX);
	if(new_p == NULL) return false;
	while((en = readdir(d)) != NULL){
		sprintf(new_p, "%s%s", p, en->d_name);
		if(strcmp(en->d_name, ".") && strcmp(en->d_name, "..")){
			if(stat(new_p, &finfo) == -1){
				int save_errno = errno;
				free(new_p);
				errno = save_errno;
				return false;
			}else{
				*size += finfo.st_size;
				if(S_ISDIR(finfo.st_mode)){
					sprintf(new_p, "%s/", new_p);
					if(!izmeri_velicinu_foldera(new_p, size)){
						int save_errno = errno;
						free(new_p);
						errno = save_errno;
						return false;
					}
				}
			}
		}
	}
	free(new_p);
	return true;
}


int daj_info_menjanja (const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf){
	UNUSED(ftwbuf);
	UNUSED(typeflag);
	uintmax_t trenutno = time(NULL);
	uintmax_t razlika = trenutno - sb->st_ctime;
	
	if(razlika / SEKUNDI_U_DANU < osDays)
		printf("%-80s\t%jd(%jd)\n", fpath, razlika / SEKUNDI_U_DANU, razlika);
	return 0;
}

int daj_fajl_ext(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf){
	UNUSED(sb);
	if(FTW_F == typeflag){
		char *pntr = strrchr(fpath + ftwbuf->base, '.');
		if(pntr != NULL && !strcmp(pntr, OsExt)) printf("%-80s\n", fpath);
	}
	return 0;
}

