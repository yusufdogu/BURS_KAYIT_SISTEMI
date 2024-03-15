#include <stdio.h>//standart giriş-çıkış fonksiyonları için
#include "BURS.h"//bu kod için bölümlerin prototiplerinin bulunduğu başlık kütüphanemiz
#include <string.h>//yazı dizileri,yazı karşılaştırmaları ve yazı uzunluğunu ölçme fonksiyonları için
#include <unistd.h>//sleep fonksiyonunu kullanabilmek için bu fonksiyonun işlevi ileride açıklanıyor
struct ogrenci{
    char tckn[11];//öğrenci kayıt işlemleri dosyaya yazma işlemlerini bu struct ile yönlendireceğiz
    double burs;
};
int main() {
    int v=menu();//yönlendirme ekranı bu şekilde switch case yapısı kullanıldı
    switch (v) {
        case 1:
            kayit();
            break;
        case 2:
            odeme();
            break;
        case 3:
            odenen();
            break;
        case 4:
            odenmeyen();
            break;
        case 5:
            sifirla();//kodu yazarken hataları düzenlemek için yaptığım fonksiyon yönledirme anlamında bir anlamı yoktur
            break;
        default:
            printf("\tYANLIS NUMARALANDIRMA\n");
            sleep(3);//istenilen geçiş öncesi duraklatma şeklinde düşünülebilir burada 3 saniye duraklatılır
            main();
    }

}
int menu(){
    int v;/*kodun her bloğunda main a geri dönüş veya fonksiyon devamı yönlendirmesi v ile yapılıyor,aslında global olarak belirlemiştim
          ama bazı yerlerde bu durum sonsuz döngü veya bitirme dururmlarına dönüştüğü için her fonksiyon içinde ayrıca belirledim*/
    printf("\n\tBURS YONLENDIRME PROGRAMINA HOŞGELDINIZ\n");
    printf("\t1->OGRENCI KAYIT\n\t2->BURS ODEME\n\t3->ODENENLERI LSITELE\n\t4->ODENMEYENLERI LISTELE\n");
    scanf("%d",&v);
    return v;//yönlendirmeler sunulup verilen karar main e geri döndürülüp işleniyor

}
void kayit() {
    int v;
    int flag;
    FILE *kayitf = fopen("kayit.txt", "a+");/*metin dosyası şeklinde hem okuma hem de yazma modunda açılıyor ve okuma modunda
                                                           açılma sebebi eğer önceden kayıtlı bulunan öğrenci tekrar girilirse bu durumu engellemek
                                                           için yazma modu zaten kayıt edilmek istenilen öğrenci içindir*/
    if (kayitf == NULL) {/*klasik dosya içi kontrolü genel olarak kodun her dosya açılma kısmında yapılmalıdır ama burada anlaşılacağı düşüncesiyle
                         sadece burada yaptım*/
        printf("DOSYA ACILMA HATASI");
        sleep(3);
        main();
    }
    struct ogrenci kayits;//kayıt için açılan ogrenci boyutundaki struct
    struct ogrenci kayitfs;//dosyadan alınıp karşılaştırma yapmak için açılan ogrenci boyutundaki struct
    printf("\tOGRENCI KAYIT BOLUMUNDESINIZ\n\tKIMLIK NO VE BURS MIKTARINI GIRINIZ\n");
    scanf("%s%lf", kayits.tckn, &kayits.burs);
    if (strlen(kayits.tckn) != 11) {/*burada strlen fonksiyonu bir stringin uzunluğunu ölçüyor manuel olarak da yapılabilir
                                        '\0' olana kadar diziyi ilerletip for döngüsünde uzunluğu bulabiliriz zaten bu da strlen
                                        fonksiyonunun yaptığı işle aynıdır*/
        printf("\tYANLIS HANELI TC NUMARASI KAYIT EKRANINA GERI DONULUYOR...\n");
        sleep(3);
        kayit();
    }
    fseek(kayitf, 0, SEEK_SET);/*her kayıt yapılmak istenince dosyayı başa konumlandırıp önceden kayıtlı mı değil mi kıyası
                                                 doğru hale gelmiş oluyor çünkü her döngü sonunda dosya işlemi sona yerleşir*/
    flag = 0; //daha önceden kayıtlı mı değil mi durumunu öğrenebilmek için yapılıyor
    while (fscanf(kayitf, "%s %lf", kayitfs.tckn, &kayitfs.burs) != EOF) {//dosyanın sonuna kadar dosyadan veri alıyoruz
        if (strcmp(kayits.tckn, kayitfs.tckn) == 0) {/*girilen tckno ile dosyadakiler baştan sona kıyaslanıyor ve strcmp fonksiyonu eğer iki dizgi birbirine
                                                     eşit ise 0 eşit değilse 0dan farklı bir değer döndürür*/
            flag = 1;/*eğer birbirlerine eşit dizgi bulursa ki bu zaten önceden kayıtlı bir tckno girildiğini ifade eder flag=1 yapıp döngüyü bitiriyoruz
                     break yapmazsak bir sonrakı dizgi eşit olmayabilir ve sorun ortaya çıkar*/
            break;
        }
    }
    if (flag) {//eğer flag=1 olursa if e giriliyor if(flag==1) yazmak da aynı anlama gelmektedir
        printf("\t%s TCKNO'LU OGRENCI ZATEN KAYITLIDIR YONLENDIRME EKRANINA GERI DONULUYOR...\n",kayits.tckn);
        fclose(kayitf);
        sleep(3);
        main();
    }
    else {
        fprintf(kayitf, "%s %lf\n", kayits.tckn, kayits.burs);/*eğer daha önceden kayıtlı değil ve 11 haneli tckno girildiyse burs miktarı ve tckno
                                                                            dosyanın içerisine kayıt ediliyor*/
        fclose(kayitf);
        printf("\t%s KAYIT TCKNO'LU OGRENCI KAYIDI TAMAMLANMISTIR ANA MENUYE DONMEK ICIN 1 KAYIT YAPMAYA DEVAM ETMEK ICIN 0 A TUSLAYINIZ\n",kayits.tckn);
        scanf("%d", &v);
        if (v == 1)
            main();
        else if (v == 0)
            kayit();
        else
            printf("\tYANLIS YONLENDIRME ANA MENUYE YONLENDIRILIYORSUNUZ\n");
            sleep(3);
            main();
    }
}

void odeme(){
    int v;
    char tckno[11];//ödenmek istenen kişinin tckno sunu öğrenebilmek için char türden dizi
    FILE *kayitf = fopen("kayit.txt", "r");//kayıtlı öğrenci mi deiğil mi kontrolü için okuma modunda açıyoruz
    FILE *odef=fopen("ode.txt","a+");//bulunursa ve daha önceden bursu ödenmemişse kontrolü için iki modda açılıyor
    printf("\tOGRENCI BURS ODEME BOLUMUNDESINIZ ODEMEK ISTENILEN OGRENCININ TCKN'NI GIRINIZ\n");
    scanf("%s",tckno);
    struct ogrenci kayitfs;//dosyadan alınıp karşılaştırma yapmak için açılan ogrenci boyutundaki struct
    if (strlen(tckno) != 11) {//11 hane kontrolü
        printf("\tYANLIS HANELI TC NUMARASI KAYIT EKRANINA GERI DONULUYOR...\n");
        kayit();
    }
    fseek(kayitf,0,SEEK_SET);//kayit fonksiyonunda açıklanan sebepten ötürü başa konumlandırıyoruz
    fseek(odef,0,SEEK_SET);
    while(!feof(odef)){//dosyanın sonuna kadar devam eder
        fscanf(odef,"%s\n",kayitfs.tckn);//bursu odenmislerin dosyasından aldığımız veriyi girilen tckno ile kıyaslaması için alıyoruz
        if(strcmp(tckno,kayitfs.tckn)==0){//karşılaştırma
            printf("\t%s TCKNO'LU KISININ BURSU ZATEN ODENMISTIR ANA MENUYE YONLENDIRILIYORSUNUZ\n",tckno);
            sleep(3);
            main();
        }
        else
            continue;/*yazılmasa da dosya imleci kaldığı yerden devam eder ama yine de eklemek istedim birde kayit fonksiyonunda flag sistemiyle yapılan iş burada
                     bu şekilde de yapılabiliyor iş biraz tercih durumudur*/
    }
    while(!feof(kayitf)){
        fscanf(kayitf,"%s\t\t\t\t\t%lf\n",kayitfs.tckn,&kayitfs.burs);/*kayıtlı kişi olup olmadığını öğreniyoruz dosya sonuna kadar bu sorgulama yapılır
                                                                                    bu1unursa zaten aşağıdaki süreci görür bulunmazsa döngü sonrasındaki süreci görür*/
        if(strcmp(tckno,kayitfs.tckn)==0){
            fprintf(odef,"%s\t\t\t\t\t%f\n",tckno,kayitfs.burs);//kayıtlıysa odenmislerin dosyasına yazdırıyoruz
            printf("\t%s TCKNO'LU OGRENCI BULUNDU VE BURS ODENMISTIR ANA MENUYE DONMEK ICIN 1 ODEME YAPMAYA DEVAM ETMEK ICIN 0 A TUSLAYINIZ\n",tckno);
            scanf("%d",&v);
            fclose(odef);
            fclose(kayitf);
            if(v==1){
                sleep(3);
                main();}
            else if(v==0)
                odeme();
            else
                printf("\tYANLIS YONLENDIRME ANA MENUYE YONLENDIRILIYORSUNUZ\n");
                sleep(3);
                main();}
    }
    fclose(odef);
    fclose(kayitf);
    printf("OGRENCI BULUNAMADI ANA MENUYE DONMEK ICIN 1 ODEME YAPMAYA DEVAM ETMEK ICIN 0 I TUSLAYINIZ\n");
    scanf("%d",&v);
    if(v==1)
        main();
    else if(v==0)
        kayit();
    else
        printf("\tYANLIS YONLENDIRME ANA MENUYE YONLENDIRILIYORSUNUZ\n");
        sleep(3);
        main();
}
void odenmeyen(){
    /*bu fonksiyon genel olarak kayıtlı kişileri baştan sona sorgular ve eğer odenmıslerin dosyasında bulunmuyorsa çıktı
    olarak yazdırır en son toplam ne kadar ödenmediği ve kişiler listesi oluşmuş olur yapılan iç içe döngü durumununun
    mantığı yukarıdaki fonksiyonlarda da açıklanıyor mantık olarak aynıdır*/
    FILE *kayitf = fopen("kayit.txt", "r");
    FILE *odef=fopen("ode.txt","r");
    int flag;
    double toplam=0;
    struct ogrenci kayitfs;
    struct ogrenci odes;
    fseek(kayitf,0,SEEK_SET);
    fseek(odef,0,SEEK_SET);
    printf("\tTCKNO\t\t\t\t\t\tBURS MIKTARI\n\n");
    while(!feof(kayitf)){
        flag=0;
        fscanf(kayitf,"%s\t\t\t\t\t%lf\n",kayitfs.tckn,&kayitfs.burs);
        while(!feof(odef)){
            fscanf(odef,"%s\t\t\t\t\t%lf\n",odes.tckn,&odes.burs);
            if(strcmp(kayitfs.tckn,odes.tckn)==0) {
                flag = 1;
                break;
            }
            else
                continue;
        }
        if(flag==0){
            printf("\t%s\t\t\t\t\t%f\n",kayitfs.tckn,kayitfs.burs);
            toplam+=kayitfs.burs;
        }
    }
    printf("\n\tTOPLAM ODENMEYEN BURS MIKTARI\t\t\t%f\n",toplam);
    sleep(3);
    fclose(kayitf);
    fclose(odef);
    printf("\tANA MENUYE YONLENDIRILIYORSUNUZ\n");
    main();
}
void odenen(){
    //basit olarak ödenmis bursların dosyasını bastan sona okur ve tutar ve kisiler listelenmis olur
    FILE *odef=fopen("ode.txt","r");
    double toplam=0;
    struct ogrenci odes;
    printf("\tTCKNO\t\t\t\t\t\tBURS MIKTARI\n\n");
    while(!feof(odef)){
        fscanf(odef,"%s\t\t\t\t\t%lf\n",odes.tckn,&odes.burs);
        printf("\t%s\t\t\t\t\t%f\n",odes.tckn,odes.burs);
        toplam+=odes.burs;
    }
    printf("\n\tTOPLAM ODENEN BURS MIKTARI\t\t\t%f\n",toplam);
    sleep(3);
    fclose(odef);
    printf("\n\tANA MENUYE YONLENDIRILIYORSUNUZ\n");
    main();

}
void sifirla(){
    //dosya sıfırlama fonksiyonudur sadece w modunda açıp kapatmak da uygundur dosyanın içerisine hiçbir şey yazmamak da
    FILE *kayitf=fopen("kayit.txt","w");
    FILE *odef=fopen("ode.txt","w");
    //fprintf(kayitf,"");
    //fprintf(odef,"");
    fclose(kayitf);
    fclose(odef);
}
/*Evet genel olarak bir burs kayıt oluşturma,kayıt odeme,bunları lısteleme gibi işlevleri yerine getiren bir kod oluşturmaya çalıştım daha öncesinde yaptığımda bazı
standart fonksiyonları kullanmadan yapmıştım (örneğin strlen,sleep gibi). Sonuç olarak bir kayıt sistemini C de nasıl yapacağım anlamında bana bir sistem örneği oluşturan
bu kod umarım ilgilenenler için faydalı olur*/