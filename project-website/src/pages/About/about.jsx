import React, {Component} from 'react';
import Navbar from '../../components/Navbar/Navbar';
import './about.css';
import '../tailwind.css'

class About extends Component {
    render() {
        return (
            <div>
                <Navbar/>
                <div className='main_container'>
                    <div className='sub_container'>
                        <div class="w-1/2 text-9xl"> hello world  </div>
                    </div>

                </div>
            </div>

        );
    }
}

export default About;