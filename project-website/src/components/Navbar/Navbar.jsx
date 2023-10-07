import React, { Component } from 'react';
import './Navbar.css'
import '../../pages/tailwind.css'

class Navbar extends Component {
  constructor(props) {
    super(props);
    this.state = {
      isOpen: false,
    };
  }

  // Function to toggle the navbar
  toggleNavbar = () => {
    this.setState({ isOpen: !this.state.isOpen });
  };

  render() {
    const { isOpen } = this.state;

    return (
      <nav className="navbar">
        <div className="navbar-container" onClick={(event) => {
          this.toggleNavbar();
        }}>
          <div className="navbar-brand">
            <a href="/">Project Elderly Care</a>
          </div>
          <div className={`navbar-menu ${isOpen ? 'active' : ''}`}>
            <ul className="navbar-items">
              <li className="navbar-item">
                <a href="/">Home</a>
              </li>
              <li className="navbar-item">
                <a href="/blogs">Blogs</a>
              </li>
              <li className="navbar-item">
                <a href="/about">About</a>
              </li>
            </ul>
          </div>
        </div>
      </nav>
    );
  }
}

export default Navbar;
