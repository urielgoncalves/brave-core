/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'
import * as ReactDOM from 'react-dom'

import shieldsDarkTheme from 'brave-ui/theme/shields-dark'
import shieldsLightTheme from 'brave-ui/theme/shields-light'
import { ThemeProvider } from 'brave-ui/theme'

import { Provider } from 'react-redux'
import { Store } from 'react-chrome-redux'
import BraveShields from './containers/braveShields'
require('../../../fonts/muli.css')
require('../../../fonts/poppins.css')

const store: any = new Store({
  portName: 'BRAVE'
})

Promise.all([
  store.ready(),
  new Promise(resolve => chrome.braveTheme.getBraveThemeType(resolve))
])
.then(([ , braveTheme ]) => {
  const mountNode: HTMLElement | null = document.querySelector('#root')
  // For an extension panel, we only need to infer theme at init
  const selectedShieldsTheme = braveTheme === 'Dark' ? shieldsDarkTheme : shieldsLightTheme
  ReactDOM.render(
    <Provider store={store}>
      <ThemeProvider theme={selectedShieldsTheme}>
        <BraveShields />
      </ThemeProvider>
    </Provider>,
    mountNode
  )
})
.catch((e) => {
  console.error('Problem mounting brave shields')
  console.error(e)
})
