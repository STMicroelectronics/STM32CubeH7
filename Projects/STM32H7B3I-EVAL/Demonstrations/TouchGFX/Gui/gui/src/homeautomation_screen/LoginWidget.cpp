#include <gui/homeautomation_screen/LoginWidget.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/hal/Types.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <BitmapDatabase.hpp>

LoginWidget::LoginWidget() :
    Container(),
    patternStarted(false),
    solutionFailed(false),
    success(false),
    tracing(false),
    solutionMinLength(0),
    failedAttempts(0),
    maxAllowedAttempts(0),
    tickCounter(0),
    completedEdges(0),
    animationEndedCallback(this, &LoginWidget::animationEnded)
{
    setTouchable(true);

    //default solution
    solution.add(0);
    solution.add(1);
    solution.add(2);
    solution.add(3);
    solutionMinLength = solution.size();

    background.setColor(Color::getColorFrom24BitRGB(0x03, 0x16, 0x1E));
    background.setPosition(0, 0, 374, 399);
    background.setAlpha(179);
    add(background);

    setWidth(background.getWidth());
    setHeight(background.getHeight());

    //Set up canvas
    myColorPainter.setColor(Color::getColorFrom24BitRGB(51, 189, 253));

    //Add header and banner
    headerTxt.setTypedText(TypedText(T_LOGIN_HEADLINE));
    headerTxt.setPosition(0, 20, getWidth(), 50);
    headerTxt.setColor(Color::getColorFrom24BitRGB(0xff, 0xff, 0xff));
    add(headerTxt);

    bannerTxt.setPosition(0, getHeight() - 50, getWidth(), 50);
    bannerTxt.setColor(Color::getColorFrom24BitRGB(255, 0, 0));
    bannerTxt.setVisible(false);
    add(bannerTxt);

    reset();
}

void LoginWidget::handleDragEvent(const touchgfx::DragEvent& evt)
{
    // Pattern cannot be started if solution failed
    if ((patternStarted && !solutionFailed && !success))
    {
        line[completedEdges].updateEnd(evt.getNewX(), evt.getNewY());
        if (!line[completedEdges].isVisible())
        {
            line[completedEdges].setVisible(true);
        }

        //If no  elements in input, that means we started away from a marker
        for (int i = 0; i < NR_OF_MARKERS; i++)
        {
            if (hitBoxes[i].getRect().intersect(evt.getNewX(), evt.getNewY()) && !input.contains(i))
            {
                //Draw additional lines along diagonals and
                if (input.size() >= 1)
                {
                    uint8_t prevVal = input[input.size() - 1];
                    if ((i == 0 || i == 2) && (prevVal == 0 || prevVal == 2))
                    {
                        activateNode(1, true);
                    }

                    if ((i == 0 || i == 6) && (prevVal == 0 || prevVal == 6))
                    {
                        activateNode(3, true);
                    }

                    if ((i == 2 || i == 8) && (prevVal == 2 || prevVal == 8))
                    {
                        activateNode(5, true);
                    }

                    if ((i == 6 || i == 8) && (prevVal == 6 || prevVal == 8))
                    {
                        activateNode(7, true);
                    }

                    //Activation of skipped contact
                    if (((i == 3 || i == 5) && (prevVal == 3 || prevVal == 5)) ||
                            ((i == 0 || i == 8) && (prevVal == 0 || prevVal == 8)) ||
                            ((i == 1 || i == 7) && (prevVal == 1 || prevVal == 7)) ||
                            ((i == 2 || i == 6) && (prevVal == 6 || prevVal == 2))
                       )
                    {
                        activateNode(4, true);
                    }

                    //activate
                    activateNode(i);
                }
            }
        }
    }
}

void LoginWidget::handleClickEvent(const touchgfx::ClickEvent& evt)
{
    //If moving success to activateSingle() RELEASED will never contain a valid solution
    if (evt.getType() == touchgfx::ClickEvent::RELEASED && !success && !tracing)
    {
        if (input.size() > 0)
        {
            if (input.size() < MIN_ACCEPTED_INPUT_SIZE)
            {
                //CANCEL CURRENT LINE IF NOT CONNECTED
                //completed edges is off by one, so we can use it for a comparison
                if (input.size() == completedEdges)
                {
                    line[completedEdges].setVisible(false);
                    line[completedEdges].invalidate();
                }

                //solution failed (too short)
                solutionFailed = true;
                touchgfx::Application::getInstance()->registerTimerWidget(this);
                showPatternTooSmallBanner();
                return;
            }
            else
            {
                processAnswer();
                if (!success)
                {
                    //CANCEL CURRENT LINE IF NOT CONNECTED - completed edges is off by one, so we can use it for a comparison
                    if (input.size() == completedEdges)
                    {
                        line[completedEdges].setVisible(false);
                    }
                    solutionFailed = true;
                    failedAttempts++;

                    //Show failure banner and begin fail timer
                    showSolutionFailureBanner();
                    touchgfx::Application::getInstance()->registerTimerWidget(this);
                    return;
                }
            }
        }
    }
    else if (evt.getType() == touchgfx::ClickEvent::PRESSED && (!solutionFailed && !success && !tracing)/* || misclick*/)
    {
        //Begin tracking
        for (int i = 0; i < NR_OF_MARKERS; i++)
        {
            if (hitBoxes[i].getRect().intersect(evt.getX(), evt.getY()) && !input.contains(i)) //we haven't already been there: we never will have, since we either failed (clear) or continued (change screen)
            {
                //begin first line
                line[0].updateStart((hitBoxes[i].getX() + hitBoxes[i].getWidth() / 2), (hitBoxes[i].getY() + hitBoxes[i].getHeight() / 2));
                line[0].updateEnd((hitBoxes[i].getX() + hitBoxes[i].getWidth() / 2), (hitBoxes[i].getY() + hitBoxes[i].getHeight() / 2));
                line[0].setVisible(true);

                //activate
                activateNode(i);
                patternStarted = true;
            }
        }
    }
}

void LoginWidget::reset()
{
    for (int i = 0; i < NR_OF_MARKERS; i++)
    {
        hitBoxes[i].setColor(touchgfx::Color::getColorFrom24BitRGB(255, 0, 0));

        //clear lines
        line[i].setVisible(false);
        line[i].setAlpha(255);

        //reset animations
        images[i].stopAnimation();
        images[i].setAlpha(255);
    }

    myColorPainter.setColor(touchgfx::Color::getColorFrom24BitRGB(51, 189, 253));
    bannerTxt.setColor(Color::getColorFrom24BitRGB(255, 0, 0));

    bannerTxt.setVisible(false);
    bannerTxt.invalidate();

    //reset stuff
    input.clear();
    completedEdges = 0;
    patternStarted = false;
    solutionFailed = false;
    success = false;
    tracing = false;

    invalidate();
}

void LoginWidget::activateNode(uint8_t id, bool completeNext) //should be able to start a new line
{
    //Only process solution if completeNext == false.

    if (!input.contains(id))
    {
        hitBoxes[id].setColor(touchgfx::Color::getColorFrom24BitRGB(0, 255, 0)); //invis, so color is unused
        hitBoxes[id].invalidate();

        images[id].startAnimation(false, true, false);
        images[id].invalidate();
        if (tracing)
        {
            images[id].setAlpha(175);
        }

        //Fixate line - Must be gray when starting the next line
        myColorPainter.setColor(touchgfx::Color::getColorFrom24BitRGB(51, 189, 253));

        line[completedEdges].updateEnd((hitBoxes[id].getX() + hitBoxes[id].getWidth() / 2), (hitBoxes[id].getY() + hitBoxes[id].getHeight() / 2));
        line[completedEdges].invalidate();

        input.add(id);

        //Inspect if solution correct only if we're not filling out missing dots.
        if (!completeNext && !tracing)
        {
            processAnswer();
            if (success)
            {
                return;
            }
        }

        //begin next line  (last possible activation will never be a skip)
        if (completedEdges < NR_OF_MARKERS - 1)
        {
            completedEdges++;

            //Fix start of next line to this
            myColorPainter.setColor(touchgfx::Color::getColorFrom24BitRGB(51, 189, 253));

            line[completedEdges].updateStart((hitBoxes[id].getX() + hitBoxes[id].getWidth() / 2), (hitBoxes[id].getY() + hitBoxes[id].getHeight() / 2));

            if (completeNext) /*complete missing dot instantly*/
            {
                //If filling in missingdots, also fill in the missing line starting from end of previous
                line[completedEdges].updateEnd(hitBoxes[input[input.size() - 1]].getX(), hitBoxes[input[input.size() - 1]].getY()); //? this should give wrong result visually
                line[completedEdges].setVisible(true);
            }
        }
        else
        {
            //No edges left to complete, force a release? or move the code from release so that we can call it manually.
            patternStarted = false;
        }
    }
}

void LoginWidget::handleTickEvent()
{
    if (!tracing && (solutionFailed))
    {
        tickCounter++;
        if (tickCounter % BANNER_DISPLAY_TIME == 0)
        {
            //reset banner if we want to have another go (success = end of the road)
            //If we want a delay before sending a message to user (view), send callback from here. Make this configurable.

            //callback win. Move this to handleTicke if we need to wait a while
            if (failureCallback && failureCallback->isValid())
            {
                failureCallback->execute();
            }

            //            if (!success)
            //           {
            //            reset(); //prolly don't need to reset if we're not staying on screen?
            //           }
            resetTimer();
        }
    }
    else if (tracing)
    {
        tickCounter++;
        if (tickCounter % TRACE_DISPLAY_TIME == 0)
        {
            reset();
            resetTimer();
        }
    }
}

void LoginWidget::setSolution(touchgfx::Vector<uint8_t, 9> solution_)
{
    for (int i = 0; i < solution_.size(); i++)
    {
        //add method to identify uniqueness
        uint8_t temp = solution_[i];
        for (int j = i + 1; j < solution_.size(); j++)
        {
            if (temp == solution_[j])
            {
                assert(false && "Solution must be unique");
            }
        }
    }
    //Set solution and update length.
    solution = solution_;
    solutionMinLength = solution.size();
}

void LoginWidget::showSolutionFailureBanner()
{
    updateBannerText(T_LOGIN_SOLUTION_INCORRECT);
}

void LoginWidget::showPatternTooSmallBanner()
{
    updateBannerText(T_LOGIN_PATTERN_TOO_SHORT);
}

void LoginWidget::showHintBanner()
{
    updateBannerText(T_LOGIN_HINT);
}

void LoginWidget::showSolutionSuccessBanner()
{
    bannerTxt.setColor(Color::getColorFrom24BitRGB(51, 189, 253));
    updateBannerText(T_LOGIN_SOLUTION_CORRECT);
}

void LoginWidget::setBitmapsAndHitBoxes(BitmapId begin, BitmapId end, uint16_t hitboxWidth, uint16_t hitboxHeight)
{
    uint8_t index = 0;
    for (int i = 0; i < NR_OF_MARKERS / 3; i++)
    {
        for (int j = 0; j < NR_OF_MARKERS / 3; j++)
        {
            hitBoxes[index].setPosition(CANVAS_X_BEGIN + j * DISTANCE_BETWEEN_MARKERS, CANVAS_Y_BEGIN + i * DISTANCE_BETWEEN_MARKERS, hitboxWidth, hitboxHeight);
            add(hitBoxes[index]);
            hitBoxes[index].setVisible(false);
            index++;
        }
    }

    //Hitbox setup. The pos for these should be based on the position of the images. And if the hit boxes are larger then the images...
    index = 0;
    for (int i = 0; i < NR_OF_MARKERS / 3; i++)
    {
        for (int j = 0; j < NR_OF_MARKERS / 3; j++)
        {
            images[index].setBitmaps(begin, end);
            images[index].setUpdateTicksInterval(1);
            images[index].setDoneAction(animationEndedCallback);
            images[index].setXY(CANVAS_X_BEGIN + j * DISTANCE_BETWEEN_MARKERS, CANVAS_Y_BEGIN + i * DISTANCE_BETWEEN_MARKERS);
            add(images[index]);
            index++;
        }
    }

    //add lines on top
    for (int i = 0; i < NR_OF_MARKERS; i++)
    {
        line[i].setPosition(0, 0, 2 * DISTANCE_BETWEEN_MARKERS + HITBOX_SIZE + CANVAS_X_BEGIN + 50, 2 * DISTANCE_BETWEEN_MARKERS + HITBOX_SIZE + CANVAS_Y_BEGIN + 50);
        line[i].setLineWidth(TRACE_LINE_THICKNESS);
        line[i].setPainter(myColorPainter);
        line[i].setLineEndingStyle(touchgfx::Line::BUTT_CAP_ENDING);
        line[i].setVisible(false);
        add(line[i]);
    }
}

void LoginWidget::showTrace(touchgfx::Vector<uint8_t, 9> solution_)
{
    reset();
    tracing = true;
    failedAttempts = 0;
    for (int i = 0; i < solution_.size(); i++)
    {
        //activate nodes
        activateNode(solution_[i]);

        //only draw lines when necessary.
        if (solution_.size() > 1 && i < solution_.size() - 1)
        {
            line[i].updateStart((hitBoxes[solution_[i]].getX() + hitBoxes[solution_[i]].getWidth() / 2), (hitBoxes[solution_[i]].getY() + hitBoxes[solution_[i]].getHeight() / 2));
            line[i].updateEnd((hitBoxes[solution_[i + 1]].getX() + hitBoxes[solution_[i + 1]].getWidth() / 2), (hitBoxes[solution_[i + 1]].getY() + hitBoxes[solution_[i + 1]].getHeight() / 2));
            line[i].setVisible(true);
            line[i].setAlpha(50);
        }
    }

    showHintBanner();

    //begin timer

    touchgfx::Application::getInstance()->registerTimerWidget(this);
}

void LoginWidget::setMaxAllowedAttempts(uint8_t attempts)
{
    maxAllowedAttempts = attempts;
}

void LoginWidget::showBannerContent(const char* text, uint8_t ms)
{
    //This method copies incoming string to buffer for x ms.
}

void LoginWidget::animationEnded(const AnimatedImage& source)
{
    //This method is called when markers are done animating.

    //No implementation
}

void LoginWidget::updateBannerText(TypedTextId textId)
{
    bannerTxt.setTypedText(TypedText(textId));
    bannerTxt.moveTo(getTextCenterXCoord(bannerTxt), bannerTxt.getY());
    bannerTxt.setVisible(true);
    bannerTxt.invalidate();
}

uint16_t LoginWidget::getTextCenterXCoord(TextArea& ta)
{
    return ((getWidth() / 2) - (ta.getWidth() / 2));
}

void LoginWidget::processAnswer()
{
    /////////////////////////////////////////////////////
    //   PROCESS SOLUTION for correct answer
    //
    success = true;
    if (input.size() != solution.size())
    {
        success = false;
    }
    else if (input.size() == solution.size())
    {
        for (int i = 0; i < input.size(); i++)
        {
            if (input[i] != solution[i])
            {
                success = false;
                break;
            }
        }
    }

    //If pattern approved, display static banner
    if (success)
    {
        failedAttempts = 0;
        showSolutionSuccessBanner();
        if (successCallback && successCallback->isValid())
        {
            successCallback->execute();
        }
    }
}

void LoginWidget::resetTimer()
{
    tickCounter = 0;
    touchgfx::Application::getInstance()->unregisterTimerWidget(this);
}

void LoginWidget::setBannerPosition(uint16_t x, uint16_t y)
{
    //This method sets the position of the banner
    //No implementation
}
