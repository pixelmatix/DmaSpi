#ifndef CHIPSELECT_H
#define CHIPSELECT_H

#include <core_pins.h>

/** \brief An abstract base class that provides an interface for chip select classes.
**/
class AbstractChipSelect
{
	public:
    /** \brief Called to select a chip. The implementing class can do other things as well.
    **/
    virtual void select() = 0;

    /** \brief Called to deselect a chip. The implementing class can do other things as well.
    **/
    virtual void deselect() = 0;

    /** \brief the virtual destructor needed to inherit from this class **/
		virtual ~AbstractChipSelect() {}
};


/** \brief "do nothing" chip select class **/
class DummyChipSelect : public AbstractChipSelect
{
  void select() override {}

  void deselect() override {}
};

/** \brief "do nothing" chip select class that
 * outputs a message through Serial when something happens
**/
class DebugChipSelect : public AbstractChipSelect
{
  void select() override {Serial.println("Debug CS: select()");}
  void deselect() override {Serial.println("Debug CS: deselect()");}
};

/** \brief An active low chip select class. This also configures the given pin.
**/
class ActiveLowChipSelect : public AbstractChipSelect
{
  public:
    /** Configues a chip select pin for OUTPUT mode,
     * manages the chip selection and a corresponding SPI transaction
     *
     * The chip select pin is asserted \e after the SPI settings are applied
     * and deasserted before the SPI transaction ends.
     * \param pin the CS pin to use
     * \param settings which SPI settings to apply when the chip is selected
    **/
    ActiveLowChipSelect(const unsigned int& pin, const SPISettings& settings)
      : pin_(pin),
      settings_(settings)
    {
      pinMode(pin, OUTPUT);
      digitalWriteFast(pin, 1);
    }

    /** \brief begins an SPI transaction selects the chip (sets the pin to low) and
    **/
    void select() override
    {
      SPI.beginTransaction(settings_);
      digitalWriteFast(pin_, 0);
    }

    /** \brief deselects the chip (sets the pin to high) and ends the SPI transaction
    **/
    void deselect() override
    {
      digitalWriteFast(pin_, 1);
      SPI.endTransaction();
    }
  private:
    const unsigned int pin_;
    const SPISettings settings_;

};

#endif // CHIPSELECT_H

