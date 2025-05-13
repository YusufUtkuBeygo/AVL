#include<iostream>
#include<algorithm>
using namespace std;

class Node
{ 
public:
	Node* left;
	Node* right;
	int data;

	Node(int d):data(d),left(nullptr),right(nullptr) {}
};


class AVLTree
{
	Node* root;

	AVLTree():root(nullptr){}

	Node* TurnedRight(Node* ROOT)
	{
		Node* leftroot = ROOT->left;
		ROOT->left = leftroot->right;
		leftroot->right = ROOT;
		return leftroot;//son durumda oluşan yeni alt ağacın kökü left root oldu ROOT'un parent nodu ile bağlantı kurmak için adresini döndürüyoruz
	}

	Node* TurnedLeft(Node* ROOT)
	{
		Node* rightroot = ROOT->right;
		ROOT->right = rightroot->left;
		rightroot->left = ROOT;
		return rightroot;
	}


	int minValue(Node* current)
	{
		if (current->left == nullptr)
			return current->data;

		return minValue(current->left);
	}

	int DirectionOfImbalance(Node* current)//Dengesizliğin ne yönde olduğunu buluyor 
	{
		if (current == nullptr)
			return 0;

		return Height(current->left) - Height(current->right);
	}

	 Node* insert(int data,Node*current)
	{
		if (current == nullptr)
			return new Node(data);

		if (current->data > data)//elemanın nerye ekleneceğine kara veriliyor 
		{
			current->left = insert(data, current->left);
			if(Height(current->left)-Height(current->right)>1)//sol tarafta dengesilzik var mı diyte kontrol ediyor 
			{
				if (current->left->data > data)//Solun soluna eklenmiş oluyor bu durumda sol-sol dengesizlği var,BİR KEZ SOLA DÖNRÜME İLE ÇÖZÜLECEKTİR
				{
					current = TurnedRight(current);
				}

				else //Solun sağına eklenmiş oluyor bu durumda sol-sağ dengesilzği var ilk önce sola sonra sağa döndürülerek sorun çözülür
				{
					current->left = TurnedLeft(current);
					current = TurnedRight(current);
				}
			}

		}

		else if (current->data < data)//elemanın nerye ekleneceğine kara veriliyor 
		{
			current->right = insert(data, current->right);
			if (Height(current->right) - Height(current->left) > 1)//sağ tarafta dengesizlik var mı diye kontrol ediyor 
			{
				if (current->right->data < data)//sağın sağına eklenmiş oluyor bu durumda sağ sağ dengesilzği var tek sola döndürme ile çözülür
				{
					current = TurnedLeft(current);
				}

				else //Bu durumda ise sağın soluna eklenmiş oluyor yani sağ-sol dengesizliği var önce sağa sonra sola döndürme ile çözülür 
				{
					current->right = TurnedRight(current);
					current = TurnedLeft(current);
				}
			}
		}

		else
			return current;

	}


	bool Search(Node*current,int data)//roottan başlayarak arama yapacak
	{
		if (current == nullptr)
		{
			cout << "There's no tree!\n";
			return false;
		}

		else if (current->data > data)
			return Search(current->left, data);

		else if (current->data < data)
			return Search(current->left, data);

		else
		{
			cout << "There's " << data << " element in this tree!\n";
			return true;
		}
	}

	int Height(Node* current)
	{
		if (current == nullptr)
			return -1;

		return 1 + max(Height(current->left), Height(current->left));
	}

	void PreOrder(Node* current)
	{
		if(current)
		cout << current->data << "-";

		PreOrder(current->left);
		PreOrder(current->left);
	}

	Node* Delete(int data, Node* current)
	{
		if (current == 0)
			return 0;

		if (current->data > data)//silinmek istenen eleman aranıyor 
		{
		//Burada fonkisyon her çağırılışında dengeli olup olmadıoğüını kontrol ettiği için pointera atama durumu var bağlar yenilenerek devam ediyor 
		current->left = Delete(data, current->left);
		}

		else if (current->data < data)//silinmek istenin eleman aranıyor 
		{
			//Aynı şekillde yukarıda olduğu gibi dengeleme kontrolü yapıldığından pointera fonkisyonu atma durumu var çünkü fonkisyonun return değeri de bir pointer ve dengeleme sonrası değişen ROOT parent ilişkişsini günceller
			current->right = Delete(data, current->right);
		}

		else//aranan eleman bulundu
		{
			if (current->left == nullptr && current->right == nullptr)//Hiç çocuğu yoksa
			{
				delete current;
				current = 0;
			}

			else if (current->left == nullptr)//sol çocuğu yoksa 
			{
				Node* dlt = current->right;
				*current = *current->right;//silinecek düğümün yerine sağ çocuğu geçiriliyor 
				delete dlt;//yer değiştirme/değer atama için oluşturduğumuz duplicate eleman siliniyor
			}

			else if (current->right == nullptr)//sağ çocuğu yoksa 
			{
				Node* dlt = current->left;
				*current = *current->left;//silinecek eleman yerine çocğuğ geçirildi
				delete dlt;//yer değiştrime/değer atama için oluşturulan duplicate eleman siliniyor
			}

			//Yer değiştirme/değer atama işlemleri düğümleri işaret edene pointerların birbirine atanması sonucu gerçekleşir


			else//iki çocğununda olduğu durum 
			{
				current->data = minValue(current->right);//minValue bulunduğu düğümden itibaren sdaece sola giderek en küçük elemanı buluyor
				Delete(current->data, current->right);
				//Silinecek elemanın değeri ile en küçük değeri değiştirdik ardındanda gerçek en küçük elemanı silmek için bir daha Delete fonksiyonunu çağırdık
			}

			//silme işlemi sonrası dengeleme için kontrol yapıalcak 


			int balance = DirectionOfImbalance(current);

			if (balance > 1)//solda dengesizlik var 
			{

				if (DirectionOfImbalance(current->left) >= 0)//sol-sağ sonucu pozitifse demekki solun solunda bir çocuğu var bu da sol-sol dengesizlğini gösterir
				{
					return TurnedRight(current);
				}

				else//Sol-sağ solun solunda değil de sağında çocuk var demektir bu da sol sağ dengesizlğini bildirir
				{
				    current->left = TurnedLeft(current->left);
				     return TurnedRight(current);
					 //Önce sola çevirdik sonra sağa çevirdik 
				}
			}


			if (balance < -1)//sol-sağ<0 ise demekki sağda dengesizlik var 
			{
				if (DirectionOfImbalance(current->right) >= 0)//sol-sağ>0 ise demekki sağın solunda bir çocuğu var bu da sağ-saol dengesizliğini bildirir
				{
					current->right = TurnedRight(current->right);
					return TurnedLeft(current);
				}

				else//sol-sağ>0 ise sağ sağ dengesizlğini bildirir
				{
					return TurnedLeft(current);
				}

			}
		}
	}
};

int main()
{


}