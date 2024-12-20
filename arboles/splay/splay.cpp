#include <iostream>
#include <sstream>
#include <cstring>
#include <string>

using namespace std;

// Clase para nodo de la lista
template <class T>
class Node{
	public:
		T data;
		Node<T> *next;

		Node(T value){
			data = value;
			next = NULL;
		}
};

// Clase para lista enlazada
template <class T>
class List{
	public:
		Node<T> *first;
		Node<T> *last;
		int size;

		List(){
			first = NULL;
			last = NULL;
			size = 0;
		}

		void insertFirst(T);
		void deleteFirst();
};


template <class T>
void List<T>::insertFirst(T value){
	Node<T> *newNode = new Node<T>(value);

	if(first == NULL){
		first = newNode;
		last = newNode;
	}

	else{
		newNode->next = first;
		first = newNode;
	}

	size++;
}


template <class T>
void List<T>::deleteFirst(){
	if(first != NULL){
		Node<T> *aux = first;
		first = first->next;
		delete aux;
		size--;
	}
}

// Clase para fila
template <class T>
class QueueList{
	public:
		List<T> *lista;
		Node<T> *front;
		Node<T> *end;

		int maxSize;
		int size;

		QueueList(int max){
			lista = new List<T>();
			front = lista->first;
			end = lista->last;
			maxSize = max;
			size = 0;
		}

		void enqueue(T);
		T dequeue();
		bool isEmpty();
		bool isFull();
};


template <class T>
void QueueList<T>::enqueue(T value){
	if(!isFull()){
		Node<T> *newNode = new Node<T>(value);

		if(front == NULL){
			front = newNode;
			end = newNode;
		}

		else{
			end->next = newNode;
			end = newNode;
		}

		size++;
	}

	else{
		cout << "Queue llena" << endl;
	}
}


template <class T>
T QueueList<T>::dequeue(){
	if(!isEmpty()){
		T value = front->data;
		Node<T> *aux = front;

		front = front->next;
		delete aux;
		size--;

		return value;
	}

	else{
		cout << "Queue vacia" << endl;
		return NULL;
	}
}


template <class T>
bool QueueList<T>::isEmpty(){
	return size == 0;
}


template <class T>
bool QueueList<T>::isFull(){
	return size == maxSize;
}

// Atributos para el nodo: punteros a izquierda y derecha 
// El puntero hacia arriba (padre) es opcional
class NodeTree{	
    public: 
		int data, level;
		NodeTree *left, *right, *up;
	
		NodeTree(int value){	
            data = value;
			level = 0;
			left = NULL;
			right = NULL;
			up = NULL;
		}
};


class SplayTree{
	public:
		NodeTree *root;

		SplayTree(int value){
			root = new NodeTree(value);
		}

		NodeTree* search(int);
		void insert(int);
		void remove(int);

		// Mostrar el arbol
		void levelorder(NodeTree*); // BFS

		// Zig Zag
		NodeTree* zig(NodeTree*);
		NodeTree* zag(NodeTree*);

		// Funcion de splay
		void splay(NodeTree*);

		void update(NodeTree*, int);
};

// Busqueda de un valor en el arbol
// Complejidad: O(log n) si el arbol esta balanceado, O(n) en el peor de los casos
NodeTree* SplayTree::search(int value){	
    // Asigna un nodo que nos sirva para recorrer el arbol 
	// aux inicia como  la raiz
	NodeTree *aux = this->root; 

	// mientras que aux no sea null... 
	while(aux != NULL){	
        // el valor buscado es menor que el del nodo actual??  
		// desplazate al nodo de su izquierda
		if(value < aux->data){
            aux = aux->left;
		}
		// el valor buscado es mayor que el del nodo actual??  
		// desplazate al nodo de su derecha
		else if(value > aux->data){	
            aux = aux->right;
		}
		// si no es mayor, ni menor, debe ser igual... 
		else{
            cout << "Valor " << value << " encontrado" << endl;
			splay(aux);
            return aux;
		}
	} 

	// Caso en que el valor no se encuentra en el arbol
    cout << "Valor " << value << " no encontrado" << endl;
	return NULL;
}


// Insercion de un valor en el arbol
// Complejidad: O(log n) si el arbol esta balanceado, O(n) en el peor de los casos
void SplayTree::insert(int value){	
    // Comienza por verificar que el valor que se quiere agregar no exista en el arbol... 
	if(this->search(value) == NULL){  
        // Crea el nodo nuevo
		NodeTree *newNode = new NodeTree(value);

		// Inicia un proceso similar al de la busqueda... 
		// Necesitas encontrar a un nodo que pueda ser el padre del nuevo
		// Inicia desde la raiz... 
		NodeTree *aux = this->root;

		// mientras aux no sea null... 
		// agrega un return cuando consigas agregar el nodo nuevo... 
		while(aux != NULL){
			// Actualiza el nivel del nodo
			newNode->level = newNode->level + 1;

            // revisa a que lado de aux deberia quedar el nodo nuevo
			// Va a la izquierda si es menor que el valor en aux
			// y a la derecha si no
			if (value < aux->data){	
                // Revisa si el nodo nuevo puede ser el hijo izquierdo de aux
                if(aux->left == NULL){
                    aux->left = newNode;
                    newNode->up = aux;
                    break;
                }  

				// si no, continua buscando un lugar a la izauierda de aux
                else{
                    aux = aux->left;
                }
			}

			else{	
                // Mismo proceso, pero para la derecha
                if(aux->right == NULL){
                    aux->right = newNode;
                    newNode->up = aux;
                    break;
                }

                else{
                    aux = aux->right;
                }
			}
		}

		// Realiza el splay del nodo nuevo
		splay(newNode);
	}
	
    else{
        cout << "El nodo ya existe" << endl;
	}

	// search(value);
}

// Elimina un nodo del arbol
// Complejidad: O(log n) si el arbol esta balanceado, O(n) en el peor de los casos
void SplayTree::remove(int value){
	NodeTree *aux = search(value);

	if(aux != NULL){
		NodeTree *parent = aux->up;

		// El nodo a eliminar es una hoja
		if(aux->left == NULL && aux->right == NULL){
			// Verifica si la hoja es izquierda o derecha
			if(aux->data < parent->data){
				// Si es izquierda, elimina el puntero izquierdo en el padre
				parent->left = NULL;
			}

			else{
				// Si es derecha, elimina el puntero derecho en el padre
				parent->right = NULL;
			}

			// Elimina el nodo
			delete aux;
		}

		// El nodo a eliminar tiene un hijo
		else if(aux->left == NULL || aux->right == NULL){
			// Verifica si el nodo a eliminar tiene un hijo izquierdo o derecho
			if(aux->left != NULL){
				// Verifica si el nodo a eliminar es izquierdo o derecho
				if(aux->data < parent->data){
					// Si es izquierdo, el hijo izquierdo del padre apunta al hijo izquierdo del nodo a eliminar
					parent->left = aux->left;
				}

				else{
					// Si es derecho, el hijo derecho del padre apunta al hijo izquierdo del nodo a eliminar
					parent->right = aux->left;
				}

				// Enlaza al hijo con el nuevo padre
				aux->left->up = parent;
			}

			else{
				// Verifica si el nodo a eliminar es izquierdo o derecho
				if(aux->data < parent->data){
					// Si es izquierdo, el hijo izquierdo del padre apunta al hijo derecho del nodo a eliminar
					parent->left = aux->right;
				}

				else{
					// Si es derecho, el hijo derecho del padre apunta al hijo derecho del nodo a eliminar
					parent->right = aux->right;
				}

				// Enlaza al hijo con el nuevo padre
				aux->right->up = parent;
			}
			
			// Elimina el nodo
			delete aux;
		}

		// El nodo a eliminar tiene dos hijos
		else{
			// Busca el sucesor del nodo a eliminar (menor del subarbol derecho)
			NodeTree* succesor = aux->right;

			// Avanza hacia la izquierda hasta que no haya mas nodos
			while (succesor->left != NULL){
				succesor = succesor->left;
			}

			aux->data = succesor->data;

			if (succesor->right != NULL){
				if(succesor->up->left == succesor){
					succesor->up->left = succesor->right;
				}

				else{
					succesor->up->right = succesor->right;
				}

				succesor->right->up = succesor->up;
			}

			else{
				if (succesor->up->left == succesor){
					succesor->up->left = NULL;
				}

				else{
					succesor->up->right = NULL;
				}
			}

			delete succesor;
		}
	}

	else{
		cout << "El nodo no existe" << endl;
	}
}

// Muestra el arbol por niveles
// Complejidad: O(n)
void SplayTree::levelorder(NodeTree *auxroot){
    QueueList<NodeTree*> *queue = new QueueList<NodeTree*>(100);
    NodeTree *aux;

    queue->enqueue(auxroot);

    while(!queue->isEmpty()){
        aux = queue->dequeue();

        cout << aux->data << " (Padre: ";

        if(aux->up != NULL){
            cout << aux->up->data << "\t";
        }

        else{
            cout << "N" << "\t";
        }

        if(aux->left != NULL){
            cout  << "L: " << aux->left->data << "\t";
            queue->enqueue(aux->left);
        }

        else{
            cout  << "L: " << "N" << "\t";
        }

        if(aux->right != NULL){
            cout  << "R: " << aux->right->data << ")" << "\t";
            queue->enqueue(aux->right);
        }

        else{
            cout  << "R: " << "N" << ")" << "\t";
        }

        cout << "lvl: " << aux->level << endl;
    }
}

// Zig: Rotacion a la izquierda
// Complejidad: O(1)
NodeTree* SplayTree::zig(NodeTree *node) {
    NodeTree *y = node->left;

	node->left = y->right;

	if(y->right != NULL){ 
		y->right->up = node;
	}

	y->right = node;
	y->up = node->up;

	if(node->up != NULL){
		if(node->up->left == node){
			node->up->left = y;
		}
		
		else{
			node->up->right = y;
		}
	}
	
	node->up = y;

    return y;
}

// Zag: Rotacion a la derecha
// Complejidad: O(1)
NodeTree* SplayTree::zag(NodeTree *node) {
    NodeTree *y = node->right;

	node->right = y->left;

	if(y->left != NULL){
		y->left->up = node;
	}

	y->left = node;
	y->up = node->up;

	if(node->up != NULL){
		if(node->up->left == node){
			node->up->left = y;
		} 
		
		else{
			node->up->right = y;
		}
	}
	
	node->up = y;

    return y;
}

// Funcion de splay
// Complejidad: O(log n) si el arbol esta balanceado, O(n) en el peor de los casos
void SplayTree::splay(NodeTree *node) {
    while (node->up != NULL) {
        NodeTree *parent = node->up;
        NodeTree *grandparent = parent->up;

        if(grandparent == NULL){ // Se llega a root
            if(node == parent->left){ // Zig
                root = zig(parent);
            }
			
			else{ // Zag
                root = zag(parent);
            }
        } 
		
		else if(node == parent->left && parent == grandparent->left){ // Zig-Zig
            zig(grandparent);
            root = zig(parent);
        } 
		
		else if(node == parent->right && parent == grandparent->right){ // Zag-Zag
            zag(grandparent);
            root = zag(parent);
        } 
		
		else if(node == parent->left && parent == grandparent->right){ // Zig-Zag
            zig(parent);
            root = zag(grandparent);
        } 
		
		else{ // Zag-Zig
            zag(parent);
            root = zig(grandparent);
        }
    }

	update(root, 0);
}

// Actualiza el nivel de los nodos
// Complejidad: O(n)
void SplayTree::update(NodeTree *node, int level) {
    if(node == NULL){
		return;
	}
    
	node->level = level;
    update(node->left, level + 1);
    update(node->right, level + 1);
}


int main(){
	// Inserts:  1, 7, 8, 24, 30, 9.   Mostrar el arbol
	SplayTree *tree = new SplayTree(1);

	tree->insert(7);
	tree->insert(8);
	tree->insert(24);
	tree->insert(30);
	tree->insert(9);

	tree->levelorder(tree->root);
	cout << endl;

	// Delete 7. Mostrar el arbol
	tree->remove(7);

	tree->levelorder(tree->root);
	cout << endl;

	// Buscar 9.
	tree->search(9);

	tree->levelorder(tree->root);
	cout << endl;

	// Insert 5 y 10. Mostrar el arbol.
	tree->insert(5);
	tree->insert(10);

	tree->levelorder(tree->root);
}