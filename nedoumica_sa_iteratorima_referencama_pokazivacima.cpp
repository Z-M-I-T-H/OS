#include <iostream>
#include <vector>

/**
 *Верујем у једног Бога, Оца, Сведржитеља, Творца неба и земље и свега видљивог и невидљивог.
 И у једног Господа Исуса Христа, Сина Божјег, Јединородног, од Оца рођеног пре свих векова; Светлост од Светлости, Бога истинитог од Бога истинитог, рођеног, не створеног, једносушног са Оцем, кроз кога је све постало;
 Који је ради нас људи и нашега спасења сишао с небеса, и оваплотио се од Духа Светога и Марије Дјеве и постао човек;
 И Који је распет за нас у време Понтија Пилата, и страдао и погребен;
 И Који је васкрсао у трећи дан по Писму;
 И Који се узнео на небеса и седи с десне стране Оца;
 И Који ће опет доћи са славом, да суди живима и мртвима, његовом Царству неће бити краја.
 И у Духа Светог, Господа, животворног, Који од Оца исходи, Који се са Оцем и Сином заједно поштује и заједно слави, Који је говорио кроз пророке.
 У једну, свету, саборну и апостолску Цркву.
 Исповедам једно крштење за опроштење грехова.
 Чекам васкрсење мртвих.
 И живот будућег века. Амин.
 *
 * /

int main(){
	std::vector<double> v(5);
	std::vector<double>::iterator it = v.begin();	
	//std::vector<double>::iterator it = &v.front(); // ne da!!!
	//std::vector<double>::iterator& it1 = v.begin(); // ne da!!!
	//std::vector<double>::iterator* it1 = &v.begin(); // ne da!!!
	
	//vector<...>::iterator it <==> ...* it
	std::cout << &*it << '\t' << &*v.begin() << "\t" << &v.front() << std::endl; // izmanipulisano da se ispise double*, a ne std::vector<double>::iterator
	std::cout << *it << '\t' << v.front() << "\t" << *v.begin() << std::endl;
	//****************************************npr...
	//vector<vector<...>>::iterator <==> ...** it??

	//it.begin() <=/=> (... **);
	//it.begin() <==> (objekat_koji_sadrzi_clan_tipa_... * # pa imas .begin() [za koji ovo isto vazi] i front())
	//it.front() <=/=> (... *);
	//			 <==> (objekat_koji_sadrzi_clan_tipa_ # isto ono za begin() vazi i ovde samo sto ne treba da se dereferencira)
	//*red==red_ptr;

	std::vector<std::vector<double>> vv;
	vv.push_back(v);
	
	std::vector<std::vector<double>>::iterator itv = vv.begin();
	std::cout << &*itv->begin() << '\t' << &itv->front() << '\t' << &*vv.begin()->begin() << '\t' << &*vv.front().begin() << std::endl;
	std::cout << itv->front() << '\t' << *itv->begin() << '\t' << vv.begin()->front() << '\t' << vv.front().front() << std::endl;
	
	
	return 0;
}
