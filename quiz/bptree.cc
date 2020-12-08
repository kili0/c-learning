#include "bptree.h"
#include <vector>
#include <sys/time.h>

void
print_tree_core(NODE *n)
{
	printf("[");
	int i;
	NNN; DDD(n->nkey);
	for (i = 0; i < n->nkey; i++) {
		DDD(n->key[0]);
		if (!n->isLeaf) { PPP(n->chi[i]); print_tree_core(n->chi[i]); }

		printf("%d", n->key[i]);
		DDD(n->key[i]);
		if (i != n->nkey-1 && n->isLeaf) putchar(' ');
	}
	if (!n->isLeaf) print_tree_core(n->chi[n->nkey]);
	printf("]");
}

void
print_tree(NODE *node)
{
	PPP(node);
	print_tree_core(node);
	printf("\n"); fflush(stdout);
}

NODE *
alloc_leaf(NODE *parent)
{
	NODE *node;
	if (!(node = (NODE *)calloc(1, sizeof(NODE)))) ERR;
	node->isLeaf = true;
	node->parent = parent;
	node->nkey = 0;

	return node;
}

TEMP *
alloc_temp()
{
	TEMP *temp;
	if (!(temp = (TEMP *)calloc(1, sizeof(TEMP)))) ERR;
	temp->nkey = 0;

	return temp;
}

NODE *
alloc_internal(NODE *parent)
{
	NODE *node;
	if (!(node = (NODE *)calloc(1, sizeof(NODE)))) ERR;
	node->isLeaf = false;
	node->parent = parent;
	node->nkey = 0;

	return node;
}

NODE *
alloc_root(NODE *left, int rs_key, NODE *right)
{
	NODE *node;

	if (!(node = (NODE *)calloc(1, sizeof(NODE)))) ERR;
	node->parent = NULL;
	node->isLeaf = false;
	node->key[0] = rs_key;
	node->chi[0] = left;
	node->chi[1] = right;
	node->nkey = 1;

	left->parent = node;
	right->parent = node;

	return node;
}

NODE *
find_leaf(NODE *node, int key)
{
	int kid;

	if (node->isLeaf) return node;
	for (kid = 0; kid < node->nkey; kid++) {
		if (key < node->key[kid]) break;
	}

	return find_leaf(node->chi[kid], key);
}

TEMP *
insert_in_temp(TEMP *temp, int key, DATA *data)
{
	int i;
	if (key < temp->key[0]) {
		for (i = temp->nkey; i>0; i--) {
			temp->chi[i+1] = temp->chi[i];
			temp->key[i] = temp->key[i-1];
		}
		temp->chi[i+1] = temp->chi[i];
		temp->chi[0] = (NODE *) data;
		temp->key[0] = key;
	}
	else {
		for (i = temp->nkey; temp->key[i-1] > key; i--) {
			temp->chi[i] = temp->chi[i-1];
			temp->key[i] = temp->key[i-1];
		}
		temp->chi[i] = (NODE *) data;
		temp->key[i] = key;
	}
	temp->nkey = temp->nkey + 1;

	return temp;
}

TEMP *
insert_in_temp_internal(TEMP *temp, int key, NODE *left)
{
	int i;
	if (key < temp->key[0]) {
		NNN;
		for (i = temp->nkey; i>0; i--) {
			temp->chi[i+1] = temp->chi[i];
			temp->key[i] = temp->key[i-1];
		}
		PPP(left);
		temp->chi[1] = temp->chi[0];
		temp->chi[0] = left;
		temp->key[0] = key;
	}
	else {
		NNN;
		for (i = temp->nkey; temp->key[i-1] > key; i--) {
			temp->chi[i+1] = temp->chi[i];
			temp->key[i] = temp->key[i-1];
		}
		temp->chi[i+1] = left;
		temp->key[i] = key;
	}
	temp->nkey = temp->nkey + 1;

	return temp;
}

NODE *
insert_in_leaf(NODE *leaf, int key, DATA *data)
{
	int i;
	if (key < leaf->key[0]) {
		for (i = leaf->nkey; i>0; i--) {
			leaf->chi[i] = leaf->chi[i-1];
			leaf->key[i] = leaf->key[i-1];
		}
		leaf->key[0] = key;
		leaf->chi[0] = (NODE *)data;
	}
	else {
    // Insert the new key
		for (i = leaf->nkey; leaf->key[i-1] > key; i--) {
			leaf->chi[i] = leaf->chi[i-1];
			leaf->key[i] = leaf->key[i-1];
		}
		leaf->key[i] = key;
		leaf->chi[i] = (NODE *)data;
	}
  // Increment the number of keys
	leaf->nkey++;

	return leaf;
}

NODE *
insert_in_internal(NODE *parent, NODE *left, int rs_key, NODE *right)
{
	int i;
	if (rs_key < left->key[0]) ERR;
	else {
		for(i = parent->nkey; (parent->key[i-1] > rs_key && i > 0); i--) {
			parent->chi[i+1] = parent->chi[i];
			parent->key[i] = parent->key[i-1];
		}
		parent->key[i] = rs_key;
		parent->nkey++;

		parent->chi[i+1] = right;
		parent->chi[i] = left;
	}
	return parent;
}

NODE *
insert_in_parent(NODE *left, int key, NODE *right, DATA *data)
{
	if(left->parent == NULL) {
		NODE *root;
		root = alloc_root(left, key, right);

		DDD(root->key[0]);  PPP(root);

		Root = root;
		return root;
	}

	NODE *node;
	node = left->parent;

	if(node->nkey < (N-1)) {
		insert_in_internal(node, left, key, right);
	}
	else {
		TEMP *keydata;
		keydata = alloc_temp();

		int i;
		PPP(node->chi[0]);
		for(i=0; i<(N-1); i++) {
			keydata->key[i] = node->key[i];
			keydata->chi[i] = node->chi[i];
			keydata->nkey++;
		}
		keydata->chi[i] = node->chi[i];

		PPP(keydata->chi[0]);  PPP(keydata->chi[1]);

		keydata = insert_in_temp_internal(keydata, key, left);

		PPP(keydata->chi[0]);  PPP(keydata->chi[1]);
		printf("keydata(parent): ");
		for(int i=0; i<keydata->nkey; i++) printf("%d ", keydata->key[i]);
		printf("\n");

		for(i=0; i<node->nkey; i++) {
			node->key[i] = 0;
			node->chi[i] = NULL;
		}
		node->chi[node->nkey] = NULL;
		node->nkey = 0;

		NODE *_node;
		_node = alloc_leaf(node->parent);

		PPP(keydata->chi[0]);

		for(i=0; i<N/2; i++) {
			node->chi[i] = keydata->chi[i];
			node->key[i] = keydata->key[i];
			node->nkey++;

			_node->chi[i] = keydata->chi[i+(N/2)];
			_node->key[i] = keydata->key[i+(N/2)];
			_node->nkey++;
		}
		node->chi[i] = keydata->chi[i];
		_node->chi[i] = keydata->chi[i+(N/2)];

		PPP(node->chi[0]);  PPP(_node->chi[0]);

		int _key = _node->key[0];
		insert_in_parent(node, _key, _node, data);
	}
	return node;
}

void
insert(int key, DATA *data)
{
	NODE *leaf;

	if (Root == NULL) {
		leaf = alloc_leaf(NULL);
		Root = leaf;
	}
	else {
		leaf = find_leaf(Root, key);
	}

	if (leaf->nkey < (N-1)) {
		insert_in_leaf(leaf, key, data);
	}
	else {
		NODE *splited_leaf;
		splited_leaf = alloc_leaf(leaf->parent);

		TEMP *keydata;
		keydata = alloc_temp();

		int i;
		for(i=0; i<N-1; i++) {
			keydata->key[i] = leaf->key[i];
			keydata->chi[i] = leaf->chi[i];
			keydata->nkey++;
		}
		keydata->chi[i] = leaf->chi[i];

		keydata = insert_in_temp(keydata, key, data);

		printf("keydata(leaf): ");
		for(int i=0; i<keydata->nkey; i++) printf("%d ", keydata->key[i]);
		printf("\n");

		splited_leaf->chi[N-1] = leaf->chi[N-1];
		leaf->chi[N-1] = splited_leaf;
		for(int i=0; i<leaf->nkey; i++) {
			leaf->key[i] = 0;
			leaf->chi[i] = NULL;
		}
		leaf->nkey = 0;

		for(int i=0; i<N/2; i++) {
			leaf->chi[i] = keydata->chi[i];
			leaf->key[i] = keydata->key[i];
			leaf->nkey++;

			splited_leaf->chi[i] = keydata->chi[i+(N/2)];
			splited_leaf->key[i] = keydata->key[i+(N/2)];
			splited_leaf->nkey++;
		}

		int _key;
		_key = splited_leaf->key[0];
		insert_in_parent(leaf, _key, splited_leaf, data);
	}
}

void
init_root(void)
{
	Root = NULL;
}

void
search_core(const int key)
{
  NODE *n = find_leaf(Root, key);
	for (int i = 0; i < n->nkey+1; i++) {
		if (n->key[i] == key) return;
	}
  cout << "Key not found: " << key << endl;
	ERR;
}

int
interactive()
{
  int key;

  std::cout << "Key: ";
  std::cin >> key;

  return key;
}

int
main(int argc, char *argv[])
{
	init_root();

  while (true) {
		insert(interactive(), NULL);
    print_tree(Root);
  }

	return 0;
}
