#include<iostream>
#include<algorithm>
using namespace std;

class Node
{
public:
	Node* left;
	Node* right;
	int data;

	Node(int d):left(nullptr),right(nullptr),data(d) {}
};


class AVLTree
{
	Node* root;

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

		if (current->data > data)//elemanın nereye ekleneceğine karar veriliyor
		{
			current->left = insert(data, current->left);
			if(Height(current->left)-Height(current->right)>1)//sol tarafta dengesizlik var mı diye kontrol ediyor
			{
				if (current->left->data > data)//Solun soluna eklenmiş oluyor bu durumda sol-sol dengesizliği var, BİR KEZ SAĞA DÖNDÜRME İLE ÇÖZÜLECEKTİR
				{
					current = TurnedRight(current);
				}

				else //Solun sağına eklenmiş oluyor bu durumda sol-sağ dengesizliği var ilk önce sol çocuk sola, sonra kök sağa döndürülerek sorun çözülür
				{
					current->left = TurnedLeft(current->left);
					current = TurnedRight(current);
				}
			}

		}

		else if (current->data < data)//elemanın nereye ekleneceğine karar veriliyor
		{
			current->right = insert(data, current->right);
			if (Height(current->right) - Height(current->left) > 1)//sağ tarafta dengesizlik var mı diye kontrol ediyor
			{
				if (current->right->data < data)//sağın sağına eklenmiş oluyor bu durumda sağ-sağ dengesizliği var tek sola döndürme ile çözülür
				{
					current = TurnedLeft(current);
				}

				else //Bu durumda ise sağın soluna eklenmiş oluyor yani sağ-sol dengesizliği var önce sağ çocuk sağa sonra kök sola döndürülür
				{
					current->right = TurnedRight(current->right);
					current = TurnedLeft(current);
				}
			}
		}

		return current;//aynı değer zaten varsa ağaç değişmeden döner
	}


	bool Search(Node*current,int data)//roottan başlayarak arama yapacak
	{
		if (current == nullptr)
			return false;

		else if (current->data > data)
			return Search(current->left, data);

		else if (current->data < data)
			return Search(current->right, data);

		else
			return true;
	}

	int Height(Node* current)
	{
		if (current == nullptr)
			return -1;

		return 1 + max(Height(current->left), Height(current->right));
	}

	void PreOrder(Node* current)
	{
		if (current == nullptr)
			return;

		cout << current->data << " ";
		PreOrder(current->left);
		PreOrder(current->right);
	}

	Node* Delete(int data, Node* current)
	{
		if (current == nullptr)
			return nullptr;

		if (current->data > data)//silinmek istenen eleman aranıyor
		{
			//Burada fonksiyon her çağırılışında dengeli olup olmadığını kontrol ettiği için pointera atama durumu var bağlar yenilenerek devam ediyor
			current->left = Delete(data, current->left);
		}

		else if (current->data < data)//silinmek istenen eleman aranıyor
		{
			//Aynı şekilde yukarıda olduğu gibi dengeleme kontrolü yapıldığından pointera fonksiyonu atama durumu var çünkü fonksiyonun return değeri de bir pointer ve dengeleme sonrası değişen ROOT parent ilişkisini günceller
			current->right = Delete(data, current->right);
		}

		else//aranan eleman bulundu
		{
			if (current->left == nullptr && current->right == nullptr)//Hiç çocuğu yoksa
			{
				delete current;
				return nullptr;
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
				*current = *current->left;//silinecek eleman yerine çocuğu geçirildi
				delete dlt;//yer değiştirme/değer atama için oluşturulan duplicate eleman siliniyor
			}

			//Yer değiştirme/değer atama işlemleri düğümleri işaret eden pointerların birbirine atanması sonucu gerçekleşir


			else//iki çocuğunun da olduğu durum
			{
				current->data = minValue(current->right);//minValue bulunduğu düğümden itibaren sadece sola giderek en küçük elemanı buluyor
				current->right = Delete(current->data, current->right);
				//Silinecek elemanın değeri ile en küçük değeri değiştirdik ardından da gerçek en küçük elemanı silmek için bir daha Delete fonksiyonunu çağırdık
			}
		}

		//silme işlemi sonrası her düğümde dengeleme için kontrol yapılacak
		int balance = DirectionOfImbalance(current);

		if (balance > 1)//solda dengesizlik var
		{
			if (DirectionOfImbalance(current->left) >= 0)//sol-sağ sonucu pozitifse demek ki solun solunda bir çocuğu var bu da sol-sol dengesizliğini gösterir
			{
				return TurnedRight(current);
			}

			else//solun solunda değil de sağında çocuk var demektir bu da sol-sağ dengesizliğini bildirir
			{
				current->left = TurnedLeft(current->left);
				return TurnedRight(current);
				//Önce sola çevirdik sonra sağa çevirdik
			}
		}


		if (balance < -1)//sol-sağ<0 ise demek ki sağda dengesizlik var
		{
			if (DirectionOfImbalance(current->right) > 0)//sol-sağ>0 ise demek ki sağın solunda bir çocuğu var bu da sağ-sol dengesizliğini bildirir
			{
				current->right = TurnedRight(current->right);
				return TurnedLeft(current);
			}

			else//sol-sağ<=0 ise sağ-sağ dengesizliğini bildirir
			{
				return TurnedLeft(current);
			}

		}

		return current;
	}

	void Destroy(Node* current)
	{
		if (current == nullptr)
			return;

		Destroy(current->left);
		Destroy(current->right);
		delete current;
	}

public:
	AVLTree():root(nullptr){}

	~AVLTree() { Destroy(root); }

	void Insert(int data) { root = insert(data, root); }

	void Remove(int data) { root = Delete(data, root); }

	bool Contains(int data) { return Search(root, data); }

	int TreeHeight() { return Height(root); }

	void PrintPreOrder()
	{
		PreOrder(root);
		cout << "\n";
	}
};

int main()
{
	AVLTree tree;

	//Sıralı ekleme normal BST'de zincir oluşturur; AVL döndürmelerle dengede tutar
	int values[] = { 10, 20, 30, 40, 50, 25 };
	for (int v : values)
		tree.Insert(v);

	cout << "PreOrder: ";
	tree.PrintPreOrder();
	cout << "Height: " << tree.TreeHeight() << "\n";

	cout << "Contains 25: " << (tree.Contains(25) ? "yes" : "no") << "\n";
	cout << "Contains 99: " << (tree.Contains(99) ? "yes" : "no") << "\n";

	tree.Remove(40);
	cout << "After removing 40: ";
	tree.PrintPreOrder();
	cout << "Height: " << tree.TreeHeight() << "\n";

	return 0;
}
