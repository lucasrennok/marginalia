# include "Invoice.h"

# include <cstdio>

/* Cria um pedido vazio para o cliente informado. */
Invoice::Invoice(int id, const Customer &customer) : customer(customer)
{
    this->id = id;
    nBooks = 0;
}

/* Devolve o identificador do pedido. */
int Invoice::getId() const
{
    return id;
}

/* Devolve o cliente do pedido. */
Customer Invoice::getCustomer() const
{
    return customer;
}

/* Acrescenta um livro ao pedido e devolve false se ele já estiver no pedido, se a quantidade não for positiva ou se o pedido estiver cheio. */
bool Invoice::addBook(const Book &book, int quantity)
{
    /* CWE-787 - ASan, escrita fora dos limites, precisa colocar o operador condicional >= para ficar correto */
    /* CWE-20 - DAST, validação de entrada, requisito número 4, alterado o operador para <= para ficar dentro da validação de entrada */
    if (nBooks >= MAX_BOOKS || quantity <= 0)
    {
        return false;
    }

    for (int i = 0; i < nBooks; i++)
    {
        if (books[i].equals(book))
        {
            return false;
        }
    }

    books[nBooks] = book;
    quantities[nBooks] = quantity;
    nBooks++;
    return true;
}

/* Remove um livro do pedido, deslocando os seguintes uma posição para trás, e devolve false se ele não estiver no pedido. */
bool Invoice::removeBook(const Book &book)
{
    for (int position = 0; position < nBooks; position++)
    {
        if (books[position].equals(book))
        {
            /* CWE-125 - VALGRIND, leitura fora dos limites, precisa alterar a condição de saída do for para ficar no limite correto, por isso -1 */
            for (int i = position; i < nBooks - 1; i++)
            {
                books[i] = books[i + 1];
                quantities[i] = quantities[i + 1];
            }

            nBooks--;
            return true;
        }
    }

    return false;
}

/* Soma o preço de cada livro multiplicado pela sua quantidade. */
double Invoice::getTotal() const
{
    double total = 0.0;

    for (int i = 0; i < nBooks; i++)
    {
        total += books[i].getPrice() * quantities[i];
    }

    return total;
}

/* Aplica ao total o desconto do cliente, em percentual. */
double Invoice::getTotalAfterDiscount() const
{
    return getTotal() * (100 - customer.getDiscount()) / 100.0;
}

/* Compara dois pedidos pelo identificador. */
bool Invoice::equals(const Invoice &another) const
{
    return id == another.id;
}

/* Monta a representação textual do pedido, no formato Invoice[id = ?, Customer[...], total = ?], com o total já com desconto. */
std::string Invoice::toString() const
{
    char total_text[32];

    snprintf(total_text, sizeof total_text, "%.2f", getTotalAfterDiscount());
    return "Invoice[id = " + std::to_string(id) + ", " + customer.toString() + ", total = " + total_text + "]";
}
