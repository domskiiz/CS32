class Investment {
public:
    Investment(string name, int value) : m_name(name), m_value(value) { }
    string name() const { return m_name; }
    virtual bool fungible() const { return true; }
    int purchasePrice() const { return m_value; }
    virtual string description() const { return ""; }
    virtual ~Investment() { }
private:
    string m_name;
    int m_value;
};

class Painting: public Investment {
public:
    Painting(string name, int value) :Investment(name, value) { }
    virtual bool fungible() const { return false; }
    virtual string description() const {
        return "painting";
    }
    virtual ~Painting() {
        cout << "Destroying " + Investment::name() + ", a painting\n";
    }
};

class Stock: public Investment {
public:
    Stock (string name, int value, string symbol)
        :Investment(name, value), m_symbol(symbol) { }
    virtual bool fungible() const { return true; }
    virtual string description() const {
        return "stock trading as " + m_symbol;
    }
    virtual ~Stock() {
        cout << "Destroying " + Investment::name() + ", a stock holding\n";
    }
private:
    string m_symbol;

};

class House: public Investment {
public:
    House(string address, int value) : Investment(address, value) { }
    virtual bool fungible() const { return false; }
    virtual string description() const {
        return "house";
    }
    virtual ~House() {
        cout << "Destroying the house " + Investment::name() << endl;;
    }
};
