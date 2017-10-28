/*
 * 拓展c++基本类型的计算，可以让c++计算数值范围更大,精度更高。
 * 允许加,减,乘,除。
 * 
 * 性能：当被除数与除数数值相差太大时，计算会很慢。
 */
#include<stdexcept>
#include<iterator>
#include<string>

#define SWITCH(x) (x-'0')	
#define MAX_PRECISION 50	//小数点后允许最大小数数（除法用）

class Big_decimal{
	public:
		/*实数加法*/
		static std::string add(const std::string s1,const std::string s2);
		/*实数减法 s1-s2*/
		static std::string subtract(const std::string s1,const std::string s2);
		/*实数乘法*/
		static std::string multiply(const std::string s1,const std::string s2);
		/*实数除法*/
		static std::string divide(const std::string s1,const std::string s2,int precision);
	private:
		/*检查输入数字是否符合要求*/
		static void check_number(const std::string s);
		/*获取小数点在数字中的位置*/
		static int get_dot_position(const std::string s);
		/*将字符串反转*/
		static std::string reverse(std::string s);
		/*判断是否是负数*/
		static bool is_negative(std::string s);
		/*s1所代表的数是不是大于s2所代表的数*/
		static int is_greater(std::string s1,std::string s2);
		/*将负数转为正数*/
		static std::string positive(const std::string s);
		/*非负实数间加法*/
		static std::string add_v(const std::string s1,const std::string s2);
		/*非负实数减法*/
		static std::string subtract_v(const std::string s1,const std::string s2);
		/*将有小数的实数转为整数并返回小数点的位置，例12.455 返回12455 dot_pos: 3*/
		static std::string into_integer(std::string s1,int &dot_pos);
		static std::string divide_v(const std::string s1,const std::string s2,std::string &temp);
		static std::string itos(unsigned i);
		static std::string move_dot_position(const std::string s,int pos);
		static void clear_zero(std::string& s1);
};
void Big_decimal::clear_zero(std::string& s1)
{
	std::string result;
	int i = 0;
	bool flag = false;
	for(i = 0;i < s1.size();i++){
		if(s1[i] != '0')
			flag = true;
		if(s1[i] == '0' && flag == false){
			if(s1[i] == '0' && s1[i+1] == '.'){
				result += s1[0];
				flag = true;
			}
			continue;
		}
		result += s1[i];
	}
	s1 = result;
}
std::string Big_decimal::into_integer(std::string s1,int &dot_pos)
{
	bool flag = false;
	std::string temp,result;
	int pos = get_dot_position(s1);
	int i = s1.size();

	if(pos != s1.size()){
		i = s1.size()-1;
		while(s1[i] == '0')
			i--;
	}
	dot_pos = i-pos;
	if(i == s1.size())
		i--;
	while(i >= 0){
		if(s1[i] == '.'){
			i--;
			continue;
		}
		temp += s1[i];
		i--;
	}
	for(i = temp.size()-1;i >= 0;i--){
		if(temp[i] != '0')
			flag = true;
		if(flag == true)
			result += temp[i];
	}
	return result;
}
std::string Big_decimal::positive(const std::string s)
{
	if(s[0] != '-')
		return s;

	std::string temp;
	for(int i = 1;i < s.size();i++)
		temp += s[i];
	return temp;
}
bool Big_decimal::is_negative(std::string s)
{
	if(s[0] == '-')
		return true;
	return false;
}
/*返回1:s1 > s2 0:s1 == s2 -1: s1 < s2*/
int Big_decimal::is_greater(std::string s1,std::string s2)
{
	int pos1 = get_dot_position(s1);
	int pos2 = get_dot_position(s2);

	if(pos1 > pos2)
		return 1;
	else if(pos1 < pos2)
		return -1;

	for(int i = 0,j = 0;i < s1.size() && j < s2.size();i++,j++){
		if(SWITCH(s1[i]) > SWITCH(s2[j]))
			return 1;
		else if(SWITCH(s1[i]) < SWITCH(s2[j]))
			return -1;
	}
	if((s1.size()-pos1) > (s2.size()-pos2))
		return 1;
	else if((s1.size()-pos1) < (s2.size()-pos2))
		return -1;
	return 0;
}
std::string Big_decimal::reverse(std::string s)
{
	int beg = 0,end = s.size()-1;
	while(beg < end){
		char t = s[beg];
		s[beg] = s[end];
		s[end] = t;
		beg++;
		end--;
	}
	return s;
}
void Big_decimal::check_number(const std::string s) 
{
	bool flag = false;
	if(s[0] == '.')
		throw std::runtime_error("Error: "+s+": 首位置不允许出现点。");
	else if(s[0] == '0' && s[1] != '.'&& s != "0" && s.size() > 1)
		throw std::runtime_error("Error"+s+": 数字不能以零开头。");
	for(int i = 0;i < s.size();i++){
		if(s[i] == '.'&&flag == true)
			throw std::runtime_error("Error: "+s+": 位置中出现多余的点。");
		else if(s[i] == '.' && flag == false){
			flag = true;
			continue;
		}
		if(s[i] < '0' || s[i] > '9'){
			if(i == 0 && s[i] == '-')
					continue;
			throw std::runtime_error("Error: "+s+": 位置: "+itos(i)+" 出现非法字符"+s[i]+"。");
		}
	}
}
/*s必须要是正数*/
int Big_decimal::get_dot_position(const std::string s)
{
	for(int i = 0;i < s.size();i++){
		if(s[i] == '.')
			return i;
	}
	return s.size();
}
std::string Big_decimal::add_v(const std::string s1,const std::string s2)
{
	std::string result_integer,result_decimal;
	int carry = 0;
	int sum = 0;
	int pos1 = get_dot_position(s1);
	int pos2 = get_dot_position(s2);
	int gap1 = s1.size()-pos1, gap2 = s2.size()-pos2;
	/*小数部分加法*/
	for(int i = s1.size()-1,j = s2.size()-1;gap1 != 0|| gap2 != 0 ;){
		if(gap1 > gap2){
			sum = SWITCH(s1[i]) + carry;
			i--;
		}else if(gap1 < gap2){
			sum = SWITCH(s2[j]) + carry;
			j--;
		}else{
			sum = SWITCH(s1[i]) + SWITCH(s2[j]);
			i--;
			j--;
		}
		carry = sum/10;	/*获取进位*/
		result_decimal = result_decimal + (char)((sum-carry*10)+'0');
		if(gap1 != 0)		/*防止s1有小数，而s2无小数部分 或反过来*/
			gap1 = i-pos1;
		if(gap2 != 0)
			gap2 = j-pos2;
	}
	/*正整数部分加法*/
	for(int i = pos1-1, j = pos2 - 1;;i--,j--){
		if(i >= 0 && j >= 0)
			sum = SWITCH(s1[i]) + SWITCH(s2[j]) + carry;
		else if(j < 0 && i >= 0)
			sum = SWITCH(s1[i]) + carry;
		else if(j >= 0 && i < 0)
			sum = SWITCH(s2[j]) + carry;
		else 
			break;

		carry = sum/10;
		result_integer = result_integer + (char)((sum-carry*10)+'0');
	}
	if(carry > 0)
		result_integer = result_integer + (char)(carry+'0');
	if(pos1 == s1.size() && pos2 == s2.size())
		return reverse(result_integer);
	else
		return reverse(result_decimal+'.'+result_integer);
}
std::string Big_decimal::add(const std::string s1,const std::string s2)
{
	check_number(s1);
	check_number(s2);
	bool neg1 = is_negative(s1),neg2 = is_negative(s2);

	if(neg1 == true && neg2 == true){	/*s1,s2都是负数相加*/
		std::string st1 = positive(s1);
		std::string st2 = positive(s2);
		std::string temp = "-";
		temp += add_v(st1,st2);
		return temp;
	}else if(neg1 == false && neg2 == true){	/*s1是正数，s2是负数相加*/
		return subtract_v(s1,positive(s2));
	}else if(neg1 == true && neg2 == false){	/*s1是负数，s2是正数相加*/
		return subtract_v(s2,positive(s1));
	}else						/*s1,s2都是正数相加*/
		return add_v(s1,s2);
}

std::string Big_decimal::subtract_v(const std::string s1,const std::string s2)
{
	if(s1 == "0" && s2 == "0")
		return "0";
	
	std::string st1;
	std::string st2;
	std::string result_integer,result_decimal;
	int negative = is_greater(s1,s2);
	if(negative == -1){				/*让较大的数减去较小的数*/
		st1 = s2;
		st2 = s1;
	}else{
		st1 = s1;
		st2 = s2;
	}
	int pos1 = get_dot_position(st1);
	int pos2 = get_dot_position(st2);
	int carry = 0;

	/*让小数部分长度对齐，例s1:12 s2:13.999 ----> s1:12.000 s2:13.999*/
	if(pos1 != st1.size() && pos2 != st2.size()){
		if(st1.size()-pos1 < st2.size()-pos2){
			std::string temp((st2.size()-pos2)-(st1.size()-pos1),'0');
			st1 += temp;
		}else if(st1.size()-pos1 > st2.size()-pos2){
			std::string temp((st1.size()-pos1)-(st2.size()-pos2),'0');
			st2 += temp;
		}
	}else if(pos1 != st1.size() && pos2 == st2.size()){
		std::string temp((st1.size()-pos1-1),'0');
		st2 = st2+'.'+temp;
	}else if(pos1 == st1.size() && pos2 != st2.size()){
		std::string temp((st2.size()-pos2-1),'0');
		st1 = st1+'.'+temp;
	}
	int i,j;
	for(i = st1.size()-1,j = st2.size()-1;j >= 0;i--,j--){
		if(st1[i] == '.' && st2[j] == '.')
			result_decimal += '.';
		if(SWITCH(st1[i]) < SWITCH(st2[j])){
			result_decimal += (char)(SWITCH(st1[i]+10-SWITCH(st2[j]))+'0');
			if(st1[i-1] == '.'){
				result_decimal += '.';
				i--;j--;
			}
			st1[i-1] = st1[i-1]-1;			/*向高位借一*/
		}else{
			if(st1[i] == '.' && st2[j] == '.')
				continue;
			result_decimal += (char)(SWITCH(st1[i]-SWITCH(st2[j]))+'0');
		}
	}
	while(i >= 0){
		result_decimal += st1[i];
		i--;
	}
	//std::cout<<"result_decimal: "<<result_decimal<<std::endl;
	result_decimal = reverse(result_decimal);
	clear_zero(result_decimal);
	if(negative == -1)				/*s1 > s2 返回负值*/
		result_decimal = '-' + result_decimal;
	return result_decimal;
}

std::string Big_decimal::subtract(const std::string s1,const std::string s2)
{
	check_number(s1);		/*数字检查*/
	check_number(s2);
	bool neg1 = is_negative(s1),neg2 = is_negative(s2);

	if(neg1 == true && neg2 == true){				/*s1是负数,s2是负数相减*/
		std::string temp = subtract_v(positive(s1),positive(s2));
		if(is_negative(temp))
			return positive(temp);
		else
			return '-'+temp;
	}else if(neg1 == false && neg2 == true){			/*s1是正数，s2是负数相减*/
		return add_v(s1,positive(s2));
	}else if(neg1 == true && neg2 == false){			/*s1是负数，s2是正数相减*/
		return '-'+add_v(positive(s1),positive(s2));
	}else								/*s1,s2都是负数相减*/
		return subtract_v(s1,s2);
}
/*实现 12*15 1: 2*15=30,2:1*15*10=150 result:180*/
std::string Big_decimal::multiply(const std::string s1,const std::string s2)
{
	check_number(s1);
	check_number(s2);

	if(s1 == "0" || s2 == "0")
		return "0";	
	int carry,sum;
	int dot1 = 0,dot2 = 0;
	bool neg1 = is_negative(s1),neg2 = is_negative(s2);
	std::string st1 = positive(s1);
	std::string st2 = positive(s2);
	st1 = into_integer(st1,dot1);
	st2 = into_integer(st2,dot2);
	std::string temp;
	std::string result("0");

	bool flag1 = false,flag2 = false;

	for(int i = st1.size()-1;i >= 0;i--){
		carry = 0; sum = 0;
		temp = "";/*对上次计算结果清零*/
		for(int j = st2.size()-1;j >= 0;j--){
			sum = SWITCH(st1[i]) * SWITCH(st2[j]) + carry;
			carry = sum/10;
			temp = temp + (char)((sum%10)+'0'); 
		}
		
		if(carry != 0)
			temp = temp + (char)(carry+'0');
		for(int k = 0; k < st1.size()-i-1;k++)
			temp = '0'+temp;		/*高位运算乘10*/
		result = add_v(result,reverse(temp));/*将这次运算结果加到上次中*/
	}
	temp = "";
	for(int i = result.size()-1;i >= 0;i--){
		temp += result[i];
		if(result.size()-i == (dot1 + dot2))
			temp += '.';
	}
	if(neg1 == neg2)		/*同号相乘为正，异号为负*/
		return reverse(temp);
	else
		return reverse(temp+'-');
}
std::string Big_decimal::itos(unsigned i)
{
	std::string result;
	if(i == 0)
		return "0";
	while(i != 0){
		result += (char)(i%10 + '0');
		i = i/10;
	}
	return reverse(result);
}
//计算s1与s2的余数通过temp返回，计算小于s1 s2的最大倍数作为函数的返回值
std::string Big_decimal::divide_v(const std::string s1,const std::string s2,std::string &temp)
{
	std::string rtemp = s2,pre;
	for(unsigned i = 1;;i++){
		if(is_greater(s1,rtemp) == 0){
			temp = "0";
			return itos(i);
		}else if(is_greater(s1,rtemp) == 1){
			rtemp = add(rtemp,s2);
		}else if(is_greater(s1,rtemp) == -1){
			temp = subtract(s1,multiply(s2,itos(i-1)));
			return itos(i-1);
		}
	}
}
std::string Big_decimal::move_dot_position(const std::string s,int pos)
{
	std::string result;
	bool flag = false;
	for(int i = s.size()-1,j = 0;pos > 0;i--,j++){
		if(j == pos)
			result += ".";
		if(i >= 0){
			result += s[i];
		}else if(i < 0 && j < pos)
			result += "0";
		if(i < 0 && j > pos)
			break;
	}
	if(pos <= 0){
		pos = pos*-1;
		for(int i = 0;i < s.size()+pos;i++){
			if(i >= s.size())
				result += "0";
			else
				result += s[i];
		}
		return result;
	}
	if(result[result.size()-1] == '.')
		return reverse(result+"0");
	return reverse(result);
}
std::string Big_decimal::divide(const std::string s1,const std::string s2,int precision)
{	
	check_number(s1);
	check_number(s2);

	if(s2 == "0")
		throw std::runtime_error("除数不能为零。");

	if(precision > MAX_PRECISION)
		precision = MAX_PRECISION;

	int i;		/*整数除法小数点对于字符串结尾的位置*/
	bool flag = false;
	bool neg1 = is_negative(s1),neg2 = is_negative(s2);
	std::string st1 = positive(s1);
	std::string st2 = positive(s2);
	int dot1 = 0,dot2 = 0;
	st1 = into_integer(st1,dot1);
	st2 = into_integer(st2,dot2);
	int dot_move = dot1 - dot2;
	std::string result,temp = st1;

	for(i = 0;is_greater(temp,"0") != 0;){
		if(i == precision)
			break;
		if(is_greater(st1,st2) == -1){
			i++;
			st1 += "0";
		}
		result += divide_v(st1,st2,temp);
		st1 = temp;
	}
	result = move_dot_position(result,dot_move+i);
	if(neg1 == neg2)
		return result;
	else
		return "-"+result;
}
