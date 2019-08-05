class Landmark
{
  public:
    Landmark(string name) { m_name = name; }
    virtual string color() const { return "yellow"; }
    virtual string icon() const = 0;
    string name() const { return m_name; }
    virtual ~Landmark(){};

  private:
    string m_name;
};

class Hotel : public Landmark
{
  public:
    Hotel(string name) : Landmark(name) {}
    virtual string icon() const { return "bed"; }
    virtual ~Hotel() { cout << "Destroying the hotel " + this->name() + "." << endl; }
};

class Restaurant : public Landmark
{
  public:
    Restaurant(string name, int cap) : Landmark(name), m_cap(cap) {};
    virtual string icon() const { return (m_cap < 40) ? "small knife/fork" : "large knife/fork"; }
    virtual ~Restaurant() { cout << "Destroying the restaurant " + this->name() + "." << endl; }

  private:
    int m_cap;
};

class Hospital : public Landmark
{
  public:
    Hospital(string name) : Landmark(name) {}
    virtual string color() const { return "blue"; }
    virtual string icon() const { return "H"; }
    virtual ~Hospital() { cout << "Destroying the hospital " + this->name() + "." << endl; }
};