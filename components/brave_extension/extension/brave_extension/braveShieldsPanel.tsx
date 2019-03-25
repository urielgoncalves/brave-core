/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'
import * as ReactDOM from 'react-dom'

import shieldsDarkTheme from 'brave-ui/theme/shields-dark'
import shieldsLightTheme from 'brave-ui/theme/shields-light'
import { ThemeProvider } from 'brave-ui/theme'
import IBraveTheme from 'brave-ui/theme/theme-interface'

import { Provider } from 'react-redux'
import { Store } from 'react-chrome-redux'
import BraveShields from './containers/braveShields'
require('../../../fonts/muli.css')
require('../../../fonts/poppins.css')

const store: any = new Store({
  portName: 'BRAVE'
})

type BraveCoreThemeProviderProps = {
  initialThemeType?: chrome.braveTheme.ThemeType
  dark: IBraveTheme,
  light: IBraveTheme
}
type BraveCoreThemeProviderState = {
  braveCoreTheme?: chrome.braveTheme.ThemeType
}

class BraveCoreThemeProvider extends React.Component<BraveCoreThemeProviderProps, BraveCoreThemeProviderState> {
  componentWillMount () {
    if (this.props.initialThemeType) {
      this.setThemeState(this.props.initialThemeType)
    }
    chrome.braveTheme.onBraveThemeTypeChanged.addListener(this.setThemeState)
  }

  setThemeState = (braveCoreTheme: chrome.braveTheme.ThemeType) => {
    this.setState({ braveCoreTheme })
  }

  render () {
    // Don't render until we have a theme
    if (!this.state.braveCoreTheme) return null
    // Render provided dark or light theme
    const selectedShieldsTheme = this.state.braveCoreTheme === 'Dark'
                                  ? this.props.dark
                                  : this.props.light
    return (
      <ThemeProvider theme={selectedShieldsTheme}>
        {React.Children.only(this.props.children)}
      </ThemeProvider>
    )
  }
}

Promise.all([
  store.ready(),
  new Promise(resolve => chrome.braveTheme.getBraveThemeType(resolve))
])
.then(([ , braveTheme ]: [ undefined, chrome.braveTheme.ThemeType ]) => {
  const mountNode: HTMLElement | null = document.querySelector('#root')
  ReactDOM.render(
    <Provider store={store}>
      <BraveCoreThemeProvider
        initialThemeType={braveTheme}
        dark={shieldsDarkTheme}
        light={shieldsLightTheme}
      >
        <BraveShields />
      </BraveCoreThemeProvider>
    </Provider>,
    mountNode
  )
})
.catch((e) => {
  console.error('Problem mounting brave shields')
  console.error(e)
})
